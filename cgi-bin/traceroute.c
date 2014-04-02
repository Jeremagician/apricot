#include "csapp.h"
#include <stdio.h>

#define DEFAULT_DESTINATION "google.com"

int main(void)
{
	char *buf;
	char dest[MAXLINE];
	int devnull = open("/dev/null", O_WRONLY); /* On l'utilise pour rediriger la sortie d'erreur sur null */

	printf("Content-type: text/plain\n");
	fflush(stdout);
	
	if ((buf = getenv("QUERY_STRING")) != NULL && *buf)
		strcpy(dest, buf);
	else
		strcpy(dest, DEFAULT_DESTINATION);

	/* On sais qu'on processus fils hérite des descripteurs de sont père donc pas de soucis de ce côté là */
	
	if(Fork() == 0)
	{
		Dup2(devnull, STDERR_FILENO);
		execlp("traceroute", "traceroute", dest, NULL);
	}
	else
	{
		int status;
	    waitpid(-1, &status, 0);
		if(WEXITSTATUS(status) !=  EXIT_SUCCESS)
			puts("Invalid host.");
	}

	fflush(stdout);
	return EXIT_SUCCESS;
}
