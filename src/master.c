#include <apricot/master.h>
#include <apricot/pool.h>
#include <apricot/log.h>
#include <apricot/csapp.h>
#include <apricot/log.h>

#include <stdlib.h>
#include <string.h>

/* Prototypes */
static void signal_handler(int sig);
static void init_signals();

char ** arguments;

void master_start(int port, char ** argv)
{
	int listenfd;
	
	strcpy(argv[0], "apricot [master]");
	arguments = argv;

    listenfd = Open_listenfd(port);

	/* init signals */
	init_signals();

	pool_create(listenfd);

	log_info("Apricot web server started on port %i", port);

	for(;;)
		sleep(-1);
}

void master_stop()
{
	log_info("Apricot web server stopped");
	pool_destroy();
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
