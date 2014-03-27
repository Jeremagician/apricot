#include <apricot/http_header.h>
#include <apricot/csapp.h>

static char buf[MAXLINE];

void http_request_read(int fd, http_request_t * request)
{
	rio_t rio;
	
	Rio_readinitb(&rio, fd);
	Rio_readlineb(&rio, buf, MAXLINE);
}
