#include <apricot/master.h>
#include <apricot/pool.h>
#include <apricot/log.h>
#include <apricot/csapp.h>
#include <apricot/conf.h>
#include <apricot/mqueue.h>
#include <apricot/mime.h>

#include <stdlib.h>
#include <string.h>

#define POOL_SIZE 16

/* Prototypes */
static void signal_handler(int sig);
static void init_signals();

char ** arguments;

void master_start(int port, char ** argv)
{
	int listenfd;

	/* change process name */
	
	strcpy(argv[0], "apricot [master]");
	arguments = argv;
	
	/* Aucun fils n'a été créé, on peut donc quitter immediatement le programme */
	if(mime_init() < 0)
		exit(EXIT_FAILURE);
	
	/* open socket */
    listenfd = Open_listenfd(port);

	/* init signals */
	init_signals();

	pool_create(listenfd, POOL_SIZE);

	log_info("Apricot web server started on port %i", port);

	/* open message queue for administration interface */
	mqueue_start();
}

void master_stop()
{
	pool_destroy();
	mime_close();
	mqueue_stop();
	log_info("Apricot admin interface closed");
	
  	unlink(LOCK_FILE);
	
	log_info("Apricot web server stopped");
}

static void signal_handler(int sig)
{
	if(sig == SIGINT)
	{
		master_stop();
		exit(EXIT_SUCCESS);
	}
}

static void init_signals()
{
	sigset_t set;

	sigfillset(&set);
	sigdelset(&set, SIGINT);
	sigdelset(&set, SIGCHLD);
	sigprocmask(SIG_SETMASK, &set, NULL);

	signal(SIGINT,  signal_handler);
}
