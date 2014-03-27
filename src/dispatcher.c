#include <apricot/dispatcher.h>
#include <apricot/rewrite.h>
#include <apricot/dynamic.h>
#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_header.h>
#include <strings.h>

static http_request_t request;

void dispatch(int acceptfd, SA *client_addr)
{
	char uri[MAXLINE];
	int http_code;
	(void)http_code; /* Temporary hack */
	
	/* fill request with zeros */
	bzero(&request, sizeof(request));
	
	/* read request */
	http_request_read(acceptfd, &request);
	
	rewrite(uri);

	if(0 /* if dynamic */)
	{
		http_code = dynamic_serve(acceptfd, uri);
	}
	else
	{
		http_code = static_serve(acceptfd, uri);
	}
}
