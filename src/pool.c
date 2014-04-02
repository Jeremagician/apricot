#include <apricot/pool.h>
#include <apricot/csapp.h>
#include <apricot/worker.h>
#include <apricot/master.h>
#include <apricot/log.h>

#include <string.h>
#include <stdlib.h>


static int pool_size;
static pid_t *pool;
static int listenfd;

/* Workers reincarnation */
static void signal_handler(int sig);
static void reincarnate(pid_t pid);
static void reincarnate_start();
static void reincarnate_stop();

void pool_create(int lfd, int psize)
{
	int i;
	listenfd = lfd;
	pool_size = psize;

	pool = malloc(sizeof(pid_t) * pool_size);
	if(pool)
	{
		reincarnate_start();

		/* Remplissage de la pool */
		for(i = 0; i < pool_size; i++)
		{
			pid_t pid = Fork();

			if(pid == 0) // Fils
			{
				strcpy(arguments[0], "apricot [worker]");
				worker_start(listenfd);
			}
			else // Pere
				pool[i] = pid;
		}
	}
	else
	{
		log_error("Could not allocate process pool");
		exit(EXIT_FAILURE); /* Aucun fils n'a été créé, on peut donc quitter immediatement le programme */
	}
}

void pool_destroy()
{
	int i;
	reincarnate_stop();

	for(i = 0; i < pool_size; i++)
	{
		/* Demande à un worker de se terminer */
		kill(pool[i], SIGTERM);
	}

	while(waitpid(-1, NULL, 0) > 0); /* On attend la terminaison de tout les fils */
    free(pool);
}

void pool_resize(int new_size)
{
	log_debug("resizing process pool");

	if(new_size == pool_size)
		return;

	if(new_size > 1)
	{
		pid_t *rpool;
		/*
		  On n'utilise pas de realloc pour gérer
		  plus proprement les erreurs d'allocation
		*/
		rpool = malloc(sizeof(pid_t) * new_size);
		if(rpool)
		{
			int i;
			pid_t *temp = pool;
			int old_size = pool_size;

			memcpy(rpool, pool, sizeof(pid_t) * new_size); /* On copie l'ancienne pool */

			/*
			  On met à jour pool et pool_size car lors de l'appel à kill,
			  le SIGCHLD renvoyé va utiliser ces données
			 */
			pool = rpool;
			pool_size = new_size;

			if(new_size < old_size) /* Reduction de la taille */
			{
				for( i = new_size; i < old_size; i++ )
					kill(temp[i], SIGTERM); /* On demande au worker d'arrêter son travail puis de se quitter */
			}
			else /* Augmentation de la taille */
			{
				for( i = old_size; i < new_size; i++)
				{
					pid_t pid = Fork();

					if(pid == 0) // Fils
					{
						strcpy(arguments[0], "apricot [worker]");
						worker_start(listenfd);
					}
					else // Pere
						pool[i] = pid;
				}
			}

			free(temp); /* On libère l'ancienne pool */
		}
		else
			log_error("(pool_resize) Can't allocate new process pool, aborted");
	}
	else
	{
		log_error("(pool_resize) Invalid new size (%i), aborted", new_size);
	}
}

static void signal_handler(int sig)
{
	/*
	  Un worker s'est arrêté anormalement.
	  On le réincarne.
	*/
	if(sig == SIGCHLD)
	{
		pid_t pid;
		while ((pid = Waitpid(-1, NULL, 0)) > 0)
		{
			int i = 0;
			/*
			  On ne réincarne le fils, uniquement si il est dans la pool.
			  Ainsi on ne réincarne pas les fils tué lors d'un resize_pool
			 */
			while (i < pool_size) {
				if(pool[i] == pid)
				{
					log_info("Reincarnation worker pid %i", pid);
					reincarnate(pid);
					break;
				}
				i++;
			}
		}
	}
}

static void reincarnate(pid_t pid)
{
	/*
	  Recherche du processus dans la table puis
	  réincarnation.
	  Complexité en O(n) mais la process pool n'est
	  pas sensée être grande, et la réincarnation
	  n'est pas sensée se produire souvent
	 */
	int i = 0;
	while(i < pool_size)
	{
		if(pool[i] == pid)
		{
			pid_t p = Fork();

			if(p == 0)
			{
				strcpy(arguments[0], "apricot [worker]");
				worker_start(listenfd);
			}
			else
			{
				pool[i] = p;
				break; /* Quand le pere a trouvé il doit sortir */
			}
		}
		i++;
	}
}

static void reincarnate_start()
{
	/* On lie le SIGCHLD à notre signal handler,
	   ainsi si un worker s'arrête anormalement, on le relance */
	signal(SIGCHLD,  signal_handler);
}

static void reincarnate_stop()
{
	/* On arrête la réincarnation,
	   Lorsque l'on veux arrêter le server (et detruire la pool)
	   Il ne faut pas reincarner les workers
	*/
	signal(SIGCHLD,  SIG_IGN);
}
