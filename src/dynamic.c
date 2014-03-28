#include <apricot/dynamic.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/log.h>

int dynamic_serve(int fd, char * filename, char * cgiargs)
{
	char buf[MAXLINE], *emptylist[] = { NULL };

    /* Return first part of HTTP response */
    snprintf(buf, sizeof(buf), "HTTP/1.0 200 OK\r\n");
    Rio_writen(fd, buf, strlen(buf));
    snprintf(buf, sizeof(buf), "Server: Apricot Web Server\r\n");
    Rio_writen(fd, buf, strlen(buf));

    if (Fork() == 0) { /* child */
    	log_debug("cgi pid = %i started", getpid()); 
        /* Real server would set all CGI vars here */
        setenv("QUERY_STRING", cgiargs, 1);
        Dup2(fd, STDOUT_FILENO);         /* Redirect stdout to client */
        Execve(filename, emptylist, environ); /* Run CGI program */
    }
  	return HTTP_OK;
}
