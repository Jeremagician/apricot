#include <apricot/master.h>
#include <apricot/pool.h>
#include <apricot/log.h>
#include <apricot/csapp.h>
#include <stdlib.h>

/* Prototypes */
static void signal_handler(int sig);
static void init_signals();

void master_start(int port)
{
	int listenfd;
	
    listenfd = Open_listenfd(port);
	
	/* init signals */
	init_signals();
	
	pool_create(listenfd);
	
	for(;;)
		sleep(-1);
}

void master_stop()
{
	log_apricot_stop();
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
