#include <apricot/master.h>
#include <apricot/pool.h>
#include <apricot/csapp.h>
#include <stdlib.h>

/* Prototypes */
static void signal_handler(int sig);
static void init_signals();

void master_start()
{
	int listenfd;
	
	/* Listenfd */
	
	/* init signals */
	init_signals();
	
	pool_create(listenfd);
	
	for(;;)
		sleep(-1);
}

void master_stop()
{
	pool_destroy(listenfd);
}

static void signal_handler(int sig)
{
	master_stop();
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
