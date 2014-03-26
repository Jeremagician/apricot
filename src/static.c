#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>

int static_serve(int clientfd, char * uri)
{
	/* THIS IS FOR DEBUG PURPOSE ONLY */
	char buf1[MAXLINE];
    char body1[MAXBUF], body2[MAXBUF];

    /* Build the HTTP response body */
    snprintf(body1, sizeof(body1), "<html><title>Apricot Test</title>");
    snprintf(body2, sizeof(body2), "%s<body bgcolor=""ffffff"">\r\n", body1);
    snprintf(body1, sizeof(body1), "%s<h1>It's work !</h1>\r\n", body2);
    snprintf(body2, sizeof(body2), "%s<em>This is pretty apache like but, well</em>", body1);
    snprintf(body1, sizeof(body1), "%s<hr><em>Apricot Web server</em>\r\n", body2);

    /* Print the HTTP response */
    snprintf(buf1, sizeof(buf1), "HTTP/1.0 %s %s\r\n", "200", "Ok");
    Rio_writen(clientfd, buf1, strlen(buf1));
    snprintf(buf1, sizeof(buf1), "Content-type: text/html\r\n");
    Rio_writen(clientfd, buf1, strlen(buf1));
    snprintf(buf1, sizeof(buf1), "Content-length: %d\r\n\r\n", (int)strlen(body1));
    Rio_writen(clientfd, buf1, strlen(buf1));
    Rio_writen(clientfd, body1, strlen(body1));
	return HTTP_OK;
}
