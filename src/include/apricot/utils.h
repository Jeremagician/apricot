#ifndef _UTILS_
#define _UTILS_

#include <stdint.h>
#include <time.h>

/* Dan Bernstein djb2 hash function
 * http://www.cse.yorku.ca/~oz/hash.html */

uint32_t hash (char * str);

/* transforms the string str to lowercase in place */

void strlower(char * str);

/* get address of client */

struct sockaddr_in getclientaddr(int fd);

/* converts an address to string */

char * straddr(struct sockaddr_in addr);

/* get client host name */

char * get_client_hostname(struct sockaddr_in addr);

/* get http formatted date stamp */

char * http_date(struct tm *date);

#endif
