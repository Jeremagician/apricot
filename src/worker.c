#include <apricot/worker.h>
#include <apricot/csapp.h>
#include <apricot/dispatcher.h>

static int listenfd;

static void signal_handler(int sig);
static void init_signals();

void worker_start(int lfd)
{
	int client_len, confd;
	struct sockaddr_in client_addr;
	listenfd = lfd;
	init_signals();

	for(;;)
	{
		client_len = sizeof(client_addr);
		confd = accept(listenfd, (SA *)&client_addr, (socklen_t*)&client_len);
		dispatch(confd, (SA *)&client_addr);
		close(confd);
	}
}

static void signal_handler(int sig)
{
	if(sig == SIGTERM)
	{
		close(listenfd); // On ferme silencieusement listenfd
		exit(EXIT_SUCCESS);
	}
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
