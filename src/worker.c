#include <apricot/worker.h>
#include <apricot/csapp.h>
#include <apricot/utils.h>
#include <apricot/dispatcher.h>
#include <apricot/log.h>
#include <apricot/http_header.h>
#include <apricot/http_error.h>
#include <apricot/http_codes.h>

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

		if(confd != -1) /* Un client s'est connecté (descripteur de fichier valide) */
		{
			worker_lock(); /* On rend le worker insensible au sigaux pour server totalement le client */
			dispatch(confd, (SA *)&client_addr); /* Confd is closed by the dispatch function */
			worker_unlock();

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

				if(WEXITSTATUS(status) != EXIT_SUCCESS)
				{
					/*
					  Le programme s'est quitté anormalement, on renvoie une erreur 500
					 */
					http_clienterror(cgi_table[i].clientfd, HTTP_INTERNAL_ERROR, HTTP_STR(HTTP_INTERNAL_ERROR));
					log_error("cgi (%i) execution error", pid);
				}
				else
				{
					char * buf;
					struct stat sbuf;
					http_response_t response;
					int cgifd = fileno(cgi_table[i].cgifile);
					rio_t rio;
					char line[MAXLINE];
					char *field, *content;
					int size;
					(void)size;
					
					lseek(cgifd, 0, SEEK_SET);
					Rio_readinitb(&rio, cgifd);
					size = Rio_readlineb(&rio, line, MAXLINE);

					field = strtok(line, ":");
					content = strtok(NULL, ":");
					content[strlen(content)-1] = 0;
					strlower(field);
					
					if(strcmp(field, "content-type"))
					{
						http_clienterror(cgi_table[i].clientfd, HTTP_INTERNAL_ERROR, HTTP_STR(HTTP_INTERNAL_ERROR));
						log_error("cgi output expected content-type header");
						log_request(&cgi_table[i].request, HTTP_INTERNAL_ERROR);
					}
					else
					{
						fstat(cgifd, &sbuf);

						/* On crée notre entête http */
						http_response_default(&response, 1, 0, HTTP_OK);
						strcpy(response.content_type, content);
						strcpy(response.cookie_id, cgi_table[i].cookie_id);
						response.content_length = (int)sbuf.st_size-size;
						http_response_write(cgi_table[i].clientfd, &response);

						/* On map le fichier sortie */
						buf = mmap(0, sbuf.st_size-size, PROT_READ, MAP_PRIVATE, cgifd, 0);
						Rio_writen(cgi_table[i].clientfd, buf+size, sbuf.st_size-size);
						munmap(buf, sbuf.st_size-size);
						log_request(&cgi_table[i].request, HTTP_OK);
					}
				}
				/* On ferme les descripteur de fichiers et on enlève le cgi de la table */
				fclose(cgi_table[i].cgifile);
				close(cgi_table[i].clientfd);
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


void log_request(http_request_t *request, int http_code)
{
	log_info("%s %s HTTP %i.%i %s %s %i",
			 HTTP_METHOD_STR(request->method),
			 request->uri,
			 request->http_version_major,
			 request->http_version_minor,
			 straddr(request->client_address),
			 get_client_hostname(request->client_address),
			 http_code
			);
}
