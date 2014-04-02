#include <apricot/dynamic.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/log.h>
#include <apricot/worker.h>
#include <apricot/cookie.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int dynamic_serve(int fd, char * filename, char * cgiargs, char * cookie_id)
{
	char *emptylist[] = { NULL };
	pid_t pid;
	FILE* output;
	char cookie_tmp[COOKIE_ID_MAX];
	
	strcpy(cookie_tmp, cookie_id);

	if(strlen(cookie_tmp) == 0)
	{
	  cookie_create(cookie_tmp);
	}
	
	output = tmpfile();
	
	if(output !=  NULL)
	{
		if ((pid = Fork()) == 0) { /* child */
			/* Real server would set all CGI vars here */
			setenv("QUERY_STRING", cgiargs, 1);
			setenv("COOKIE", cookie_tmp, 1);
			Dup2(fileno(output), STDOUT_FILENO);         /* Redirect stdout to client */
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
