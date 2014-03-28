#include <apricot/http_error.h>
#include <apricot/http_codes.h>
#include <apricot/csapp.h>

void http_clienterror(int fd, int errnum,
                 char *shortmsg)
{
    char buf1[MAXLINE];
    char body1[MAXBUF], body2[MAXBUF];

    /* Build the HTTP response body */
    snprintf(body1, sizeof(body1), "<html><title>Apricot Error</title>");
    snprintf(body2, sizeof(body2), "%s<body bgcolor=""ffffff"">\r\n", body1);
    snprintf(body1, sizeof(body1), "%s<h1>Error %i</h1> \n%s\r\n", body2, errnum, shortmsg);
    snprintf(body2, sizeof(body2), "%s\r\n", body1);
    snprintf(body1, sizeof(body1), "%s<hr><em>The Apricot Web server</em>\r\n", body2);

    /* Print the HTTP response */
    snprintf(buf1, sizeof(buf1), "HTTP/1.0 %i %s\r\n", errnum, shortmsg);
    Rio_writen(fd, buf1, strlen(buf1));
    snprintf(buf1, sizeof(buf1), "Content-type: text/html\r\n");
    Rio_writen(fd, buf1, strlen(buf1));
    snprintf(buf1, sizeof(buf1), "Content-length: %d\r\n\r\n", (int)strlen(body1));
    Rio_writen(fd, buf1, strlen(buf1));
    Rio_writen(fd, body1, strlen(body1));
}

