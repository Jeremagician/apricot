#include <apricot/worker.h>
#include <apricot/csapp.h>
#include <apricot/dispatcher.h>
#include <apricot/log.h>

static void init_signals();
static void signal_handler(int sig);
static inline void worker_lock();
static inline void worker_unlock();
static inline int check_pending(int sig);
static inline int worker_accept(int lfd, SA *caddr, socklen_t* clen);

int cgi_table_size = 0;

void worker_start(int listenfd)
{
	int client_len, confd;
	struct sockaddr_in client_addr;
	int running = 1;

	init_signals();

	while(running)
	{
		confd = worker_accept(listenfd, (SA *)&client_addr, (socklen_t*)&client_len);

		if(confd != -1)
		{
			worker_lock();
			dispatch(confd, (SA *)&client_addr);
			worker_unlock();
			Close(confd);

			if(check_pending(SIGTERM)) /* On a reçu un SIGTERM pendant le service du client */
				running = 0;
		}
		else if(errno == EINTR) /* Accept a été interrompu par SIGTERM, on sort donc de la boucle */
			running = 0;
	}
	close(listenfd);
	exit(EXIT_SUCCESS);
}

/*
  Wrapper qui permet de gérer l'accept et les demandes de terminaison (SIGTERM)
  Par défault, lorsque l'on reçoit une interruption pendant un appel système,
  Celui ci est relancé, ici on fait en sorte que l'appel à accept sort si il reçoit
  Un SIGTERM.
 */
static inline int worker_accept(int listenfd, SA *caddr, socklen_t* clen)
{
	int confd;
	int errno_tmp;

	*clen = sizeof(*caddr);
	siginterrupt(SIGTERM, 1);
	confd = accept(listenfd, caddr, clen);
	errno_tmp = errno;
	siginterrupt(SIGTERM, 0);
	errno = errno_tmp;

	return confd;
}

static void signal_handler(int sig)
{
	if(sig == SIGCHLD)
	{
		pid_t pid;
		int status;

		while((pid = waitpid(-1, &status, 0)) > 0)
		{
			int i = 0;
			while(i < cgi_table_size && cgi_table[i].pid != pid) i++; /* On cherche l'indice du cgi dans la cgi table */

			if(i == cgi_table_size)
			{
				/*
				  Etat incohérent, par ailleurs non atteignable si l'execution est correcte.
				  Nous faisons le choix ici de se tuer pour donner du boulot au serveur de réincarnation.
				  Cela aura aussi pour effect de couper toutes les sockets et ainsi ne pas laisser le
				  client en attente de données.
				 */
				log_error("can't find cgi in table, killing myself");
				exit(EXIT_FAILURE);
			}
			else
			{
				char buf[MAXLINE];
				rio_t rio;

				log_info("cgi info pid : %i client : %i data : %i", cgi_table[i].pid, cgi_table[i].clientfd, cgi_table[i].cgifd);

				Rio_readinitb(&rio, cgi_table[i].cgifd);
				Rio_readlineb(&rio, buf, MAXLINE);
				while(Rio_readlineb(&rio, buf, MAXLINE) ==  MAXLINE)
				{
					printf("%s\n", buf);
				}

				close(cgi_table[i].cgifd);
				memcpy(&cgi_table[i], &cgi_table[cgi_table_size], sizeof(cgi_t));
				cgi_table_size--;
			}

			log_debug("cgi pid = %i reaped with status code %i", pid, WEXITSTATUS(status));
		}
	}
}

static void init_signals()
{
	sigset_t set;

	/* On masque tout les signaux sauf SIGTERM */
	sigfillset(&set);
	sigdelset(&set, SIGTERM);
	sigdelset(&set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &set, NULL);

	/* On lie notre signal handler aux signaux que l'on désire gérer */
	signal(SIGTERM, signal_handler);
	signal(SIGCHLD, signal_handler);
}

static inline void worker_lock()
{
	sigset_t set;

	/* On masque tout les signaux */
	sigfillset(&set);
	sigdelset(&set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &set, NULL);
}

static inline void worker_unlock()
{
	sigset_t set;

	/* On masque tout les signaux sauf SIGTERM */
	sigfillset(&set);
	sigdelset(&set, SIGTERM);
	sigdelset(&set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &set, NULL);
}

static inline int check_pending(int sig)
{
	/* On regarde si l'on a reçu un signal pendant le lock */
	sigset_t set;
	sigpending(&set);

   	return sigismember(&set, sig);
}
