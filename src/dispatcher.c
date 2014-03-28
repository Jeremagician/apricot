#include <apricot/dispatcher.h>
#include <apricot/rewrite.h>
#include <apricot/dynamic.h>
#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_header.h>
#include <apricot/http_error.h>
#include <apricot/http_codes.h>
#include <strings.h>

/* on met la structure qui représente le header d'une requete
en global pour ne pas avoir à l'allouer sur la pile à chaque
appel à dispatch */

static http_request_t request;

static short is_dynamic(char* uri)
{
	return strstr(uri, "cgi-bin") != NULL;
}

void dispatch(int acceptfd, SA *client_addr)
{
	int http_code;
	(void)http_code; /* Temporary hack */
	
	/* fill request with zeros */
	bzero(&request, sizeof(request));
	
	/* read request */
	int req_result = http_request_read(acceptfd, &request);
	
	if(req_result == -1)
		return;
	
	rewrite(request.uri);

	if(is_dynamic(request.uri))
	{
		http_clienterror(acceptfd, HTTP_NOT_IMPLEMENTED, "Not Implemented");
		/* http_code = dynamic_serve(acceptfd, request.uri); */
	}
	else
	{
		http_code = static_serve(acceptfd, request.uri);
	}
}
