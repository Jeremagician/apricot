#include <apricot/worker.h>
#include <apricot/csapp.h>
#include <apricot/dispatcher.h>

static int listenfd;

static void signal_handler(int sig);
static void init_signals();

void worker_start(int lfd)
{
	int client_lenn, confd;
	struct sockaddr_in client_addr;
	listenfd = lfd;
	init_signals();
	
	for(;;)
	{
		client_len = sizeof(client_addr);
		confd = Accept(listenfd, (SA *)&client_addr, (socklen_t*)&client_len);
		dispatch(confd, &client_addr);
		Close(confd);
	}
	Close(listenfd);
}

static void signal_handler(int sig)
{
	/* sig = sigterm "par construction" */
	Close(listenfd);
}

static void init_signals()
{
	sigset_t set;
	/* On masque tout les signaux sauf SIGTERM */
	sigfillset(&set);
	sigdelset(&set, SIGTERM);
	sigprocmask(SIG_SETMASK, &set, NULL);

	/* On lie notre signal handler à SIGTERM */
	signal(SIGTERM,  signal_handler);
}
