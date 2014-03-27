#include <apricot/pool.h>
#include <apricot/csapp.h>
#include <apricot/worker.h>
#include <apricot/master.h>
#include <apricot/log.h>

#include <string.h>

#define POOL_SIZE 16

static pid_t pool[POOL_SIZE];
static int listenfd;

/* Workers reincarnation */
static void signal_handler(int sig);
static void reincarnate(pid_t pid);
static void reincarnate_start();
static void reincarnate_stop();

void pool_create(int lfd)
{
	int i;
	listenfd = lfd;

	reincarnate_start();
	
	/* Remplissage de la pool */
	for(i = 0; i < POOL_SIZE; i++)
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

void pool_destroy()
{
	int i;
	
	reincarnate_stop();
	
	for(i = 0; i < POOL_SIZE; i++)
	{
		/* Demande à un worker de se terminer */
		kill(pool[i], SIGTERM);
	}

	while(waitpid(-1, NULL, 0) > 0); /* On attend la terminaison de tout les fils */
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
		while((pid = Waitpid(-1, NULL, 0)) > 0)
		{
			log_info("Reincarnation worker pid %i", pid);
			reincarnate(pid);
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
	while(i < POOL_SIZE)
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
