#include <apricot/dynamic.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/log.h>
#include <apricot/worker.h>
#include <stdio.h>

int dynamic_serve(int fd, char * filename, char * cgiargs)
{
	char *emptylist[] = { NULL };
	pid_t pid;
	int output;

	output = open("/tmp/test", O_RDWR | O_CREAT, 0600);

	if(output != -1)
	{
		if ((pid = Fork()) == 0) { /* child */
			/* Real server would set all CGI vars here */
			setenv("QUERY_STRING", cgiargs, 1);
			Dup2(output, STDOUT_FILENO);         /* Redirect stdout to client */
			Execve(filename, emptylist, environ); /* Run CGI program */
		}
		else
		{
			log_debug("cgi pid = %i started", pid);
			cgi_table[cgi_table_size].cgifd = output;
			cgi_table[cgi_table_size].clientfd = fd;
			cgi_table[cgi_table_size].pid = pid;
			cgi_table_size++;
		}

		return HTTP_OK;
	}
	else
		return HTTP_INTERNAL_ERROR;
}
