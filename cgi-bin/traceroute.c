#include "cgi.h"
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#define DEFAULT_DESTINATION "google.com"

int main(void)
{
	char *buf;
	char *dest = DEFAULT_DESTINATION;
	int devnull;

	printf("Content-type: text/plain\n");
	fflush(stdout); /* Il est très important de flush le buffer car sinon il va être écrasé par execlp */
	cgi_init();

    devnull = open("/dev/null", O_WRONLY); /* On l'utilise pour rediriger la sortie d'erreur sur null */

	if(cgi_has_get() && cgi_get_isset("dest"))
	    dest = cgi_get_value("dest");

	/* On sais qu'on processus fils hérite des descripteurs de sont père donc pas de soucis de ce côté là */
	if(fork() == 0)
	{
		//dup2(devnull, STDERR_FILENO);
		execlp("traceroute", "traceroute", dest, NULL);
		perror("erreur");
		exit(EXIT_FAILURE);
	}
	else
	{
		int status;
	    waitpid(-1, &status, 0);
		if(WEXITSTATUS(status) !=  EXIT_SUCCESS)
			puts("Invalid host.");
	}

	fflush(stdout);
	cgi_exit();
	return EXIT_SUCCESS;
}
