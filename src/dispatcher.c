#include <apricot/dispatcher.h>
#include <apricot/rewrite.h>
#include <apricot/dynamic.h>
#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_header.h>
#include <strings.h>

/* on met la structure qui représente le header d'une requete
en global pour ne pas avoir à l'allouer sur la pile à chaque
appel à dispatch */

static http_request_t request;
static http_response_t response;

void dispatch(int acceptfd, SA *client_addr)
{
	int http_code;
	(void)http_code; /* Temporary hack */
	
	/* fill request and response with zeros */
	bzero(&request, sizeof(request));
	bzero(&response, sizeof(response));
	
	/* read request */
	int req_result = http_request_read(acceptfd, &request);
	
	if(req_result == -1)
		return;
	
	rewrite(request.uri);

	if(0 /* if dynamic */)
	{
		http_code = dynamic_serve(acceptfd, request.uri);
	}
	else
	{
		http_code = static_serve(acceptfd, request.uri);
	}
}
