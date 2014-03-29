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
static http_response_t response;

int parse_uri(char *uri, char *filename, char *cgiargs);

void dispatch(int acceptfd, SA *client_addr)
{
	int http_code;
    char filename[MAXLINE], cgiargs[MAXLINE];
	int is_static;
	struct stat fs;
	
	/* fill request and response with zeros */
	bzero(&request, sizeof(request));
	bzero(&response, sizeof(response));
	
	/* read request, exit if something goes wrong during
	 request reading, error messages have been already sent
	 if any. */
	int req_result = http_request_read(acceptfd, &request);
	
	if(req_result == -1)
		return;
	
	rewrite(request.uri);

	is_static = parse_uri(request.uri, filename, cgiargs);

	printf("%s\n", filename);
	
	if (stat(filename, &fs) < 0) {
		http_code = HTTP_NOT_FOUND;
	}
	else
	{
		if(is_static)
		{
			if (!(S_ISREG(fs.st_mode)) || !(S_IRUSR & fs.st_mode))
				http_code = HTTP_FORBIDDEN;
			else
				http_code = static_serve(acceptfd, request.uri);
		}
		else
		{
			if (!(S_ISREG(fs.st_mode)) || !(S_IXUSR & fs.st_mode))
				http_code = HTTP_FORBIDDEN;
			else
				http_code = dynamic_serve(acceptfd, filename, cgiargs);
		}
	}
	
	if(http_code != HTTP_OK)
		http_clienterror(acceptfd, http_code, HTTP_STR(http_code));
}

int parse_uri(char *uri, char *filename, char *cgiargs)
{
    char *ptr;

    if (!strstr(uri, "cgi-bin")) {  /* Static content */
        strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        if (uri[strlen(uri)-1] == '/')
            strcat(filename, "home.html");
        return 1;
    }
    else {  /* Dynamic content */
        ptr = index(uri, '?');
        if (ptr) {
            strcpy(cgiargs, ptr+1);
            *ptr = '\0';
        }
        else
            strcpy(cgiargs, "");
        strcpy(filename, ".");
        strcat(filename, uri);
        return 0;
    }
}
