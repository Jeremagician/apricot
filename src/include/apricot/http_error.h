#ifndef _HTTP_ERROR_
#define _HTTP_ERROR_

void http_clienterror(int fd, int errnum,
                 char *shortmsg);

#endif 
