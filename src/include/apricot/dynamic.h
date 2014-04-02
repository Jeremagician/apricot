#ifndef _DYNAMIC_
#define _DYNAMIC_

#include <apricot/http_header.h>

int dynamic_serve(int fd, char * filename, char * cgiargs, http_request_t * request);

#endif
