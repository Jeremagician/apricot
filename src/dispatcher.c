#include <apricot/dispatcher.h>
#include <apricot/rewrite.h>
#include <apricot/dynamic.h>
#include <apricot/static.h>
#include <apricot/csapp.h>

void dispatch(int acceptfd, SA *client_addr)
{
	/*char buf[MAXLINE]; */
	char uri[MAXLINE];
	int http_code;
	(void)http_code; /* Temporary hack */
	
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
