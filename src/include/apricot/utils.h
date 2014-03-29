#ifndef _UTILS_
#define _UTILS_

#include <stdint.h>

/* Dan Bernstein djb2 hash function
 * http://www.cse.yorku.ca/~oz/hash.html */

uint32_t hash (char * str);


/* transforms the string str to lowercase in place */

void strlower(char * str);

#endif