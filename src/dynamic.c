#include <apricot/dynamic.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/log.h>
#include <apricot/worker.h>
#include <apricot/cookie.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dynamic_serve(int fd, char * filename, char * cgiargs, http_request_t *request)
{
	char *emptylist[] = { NULL };
	pid_t pid;
	FILE* output;

	output = tmpfile();
	if(output !=  NULL)
	{
		if ((pid = Fork()) == 0) { /* child */
			setenv("QUERY_STRING", cgiargs, 1);
			setenv("COOKIE", cookie_getfile(request->cookie_id), 1);
			
			if(request->method == HTTP_METHOD_POST)
			{
				char content_length[32];
				printf("%s", request->content_type);
				sprintf(content_length, "%i", request->content_length);
				setenv("CONTENT_LENGTH", content_length, 1);
				Dup2(fd, STDIN_FILENO);
			}
			
			Dup2(fileno(output), STDOUT_FILENO); /* Redirect stdout to client */
			Execve(filename, emptylist, environ); /* Run CGI program */
		}
		else
		{
			/*
			  On ajoute à la table des cgi les informations
			  sur le cgi que l'on viens de lancer.
			  Le worker actuel va donc pouvoir traiter d'autre requetes
			  pendant l'execution du cgi
			 */
			log_debug("cgi pid = %i started", pid);
			cgi_table[cgi_table_size].cgifile = output;
			cgi_table[cgi_table_size].clientfd = fd;
			cgi_table[cgi_table_size].pid = pid;
			memcpy(&cgi_table[cgi_table_size].request, request, sizeof(http_request_t));
			cgi_table_size++;
		}

		return HTTP_OK;
	}
	else
	{
		log_error("can't create temporary cgi file");
		return HTTP_INTERNAL_ERROR;
	}
}
