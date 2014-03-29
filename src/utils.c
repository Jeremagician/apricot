#include <apricot/utils.h>
#include <stdint.h>
#include <string.h>

/* Dan Bernstein djb2 hash function
 * http://www.cse.yorku.ca/~oz/hash.html */

uint32_t hash (char * str)
{
	uint32_t h = 0;
	
	while(*str)
	{
		h = (h<<5) + h + *str;
		str++;
	}
	
	return h;
}

void strlower(char * str)
{
	while(*str)
	{
		*str |= 0b100000;
		str++;
	}
}