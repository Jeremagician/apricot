#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/http_header.h>
#include <string.h>

int static_serve(int clientfd, char * uri)
{
	/* THIS IS FOR DEBUG PURPOSE ONLY */
    char body1[MAXBUF], body2[MAXBUF];
	
	static http_response_t response;
	bzero(&response, sizeof(response));
	
    /* Build the HTTP response body */
    snprintf(body1, sizeof(body1), "<html><title>Apricot Test</title>");
    snprintf(body2, sizeof(body2), "%s<body bgcolor=""ffffff"">\r\n", body1);
    snprintf(body1, sizeof(body1), "%s<h1>It's work !</h1>\r\n", body2);
    snprintf(body2, sizeof(body2), "%s<em>This is pretty apache like but, well</em>", body1);
    snprintf(body1, sizeof(body1), "%s<hr><em>Apricot Web server</em>\r\n", body2);

    /* Print the HTTP response */
	response.http_version_major = 1;
	response.http_version_minor = 0;
	response.status_code = HTTP_OK;
	strcpy(response.reason_phrase, "Ok");
	strcpy(response.content_type, "text/html");
	response.content_length = (int)strlen(body1);
	http_response_write(clientfd, &response);
	
    Rio_writen(clientfd, body1, strlen(body1));
	return HTTP_OK;
}
