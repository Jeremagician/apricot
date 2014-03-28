#include <apricot/http_header.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/http_error.h>
#include <apricot/log.h>
	
#define FIELD_MAX 50
#define BUF_SIZE METHOD_MAX + URI_MAX + 10

static char buf[BUF_SIZE];

int http_request_read(int fd, http_request_t * request)
{
	rio_t rio;
	char method[METHOD_MAX];
	char * field;
	char * content;
	
	Rio_readinitb(&rio, fd);
	
	/* read request line */
	Rio_readlineb(&rio, buf, BUF_SIZE);
	
	if(sscanf(buf, "%24s %8096s HTTP/%i.%i", method, request->uri, &request->http_version_major, &request->http_version_minor) != 4)
	{
		 http_clienterror(fd, HTTP_BAD_REQUEST, "Bad request line");
		 return -1;
	}
	
	/* method */
	if(strcasecmp(method, "GET"))
	{
		http_clienterror(fd, HTTP_NOT_IMPLEMENTED, "Apricot does not implement his method");
		return -1;
	}
	
	request->method = HTTP_METHOD_GET;
	
	/* check http version number */
	if(request->http_version_major < 0 || request->http_version_minor < 0 
		|| request->http_version_major > 9 || request->http_version_minor > 9)
	{
		http_clienterror(fd, HTTP_BAD_REQUEST, "Bad HTTP version");
		return -1;
	}
	
	log_info("%s %s HTTP/%i.%i", method, request->uri, request->http_version_major, request->http_version_minor);
	
	/* read other headers */
	Rio_readlineb(&rio, buf, BUF_SIZE);
    
    while (!(buf[0] == '\r' && buf[1] == '\n') && buf[0] != '\n')
    {	
    	/* lit chaque header */
    	/* chaque header est composé d'un champ et d'un contenu séparé par ':' */
    	field = strtok(buf, ":");
    	content = strtok(NULL, ": ");
    	
    	if(!strcasecmp(field, "Accept"))
    	{
    		strncpy(request->accept, content, ACCEPT_MAX);
    		log_info("Accept = %s", content);
    	}
    	else if(!strcasecmp(field, "Accept-Charset"))
    	{
    		strncpy(request->accept_charset, content, ACCEPT_CHARSET_MAX);
    		log_info("Accept-charset = %s", content);
    	}
    	
        Rio_readlineb(&rio, buf, BUF_SIZE);
    }
	
	return 0;
}
