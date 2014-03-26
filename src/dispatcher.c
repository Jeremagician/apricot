#include <apricot/dispatcher.h>
#include <apricot/rewrite.h>
#include <apricot/csapp.h>

void dispatch(int acceptfd, struct sockaddr_in *client_addr)
{
	char buf[MAXLINE];
	char uri[MAXLINE];
	int http_code;
	
	rewrite(uri);

	if(1 /* if dynamic */)
	{
		http_code = dynamic_serve(uri);
	}
	else
	{
		http_code = static_serve(uri);
	}
}
