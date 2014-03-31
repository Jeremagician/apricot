#include <apricot/utils.h>
#include <stdint.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <assert.h>
#include <stdio.h>
#include <time.h>

#define DATE_BUF_MAX 255

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

struct sockaddr_in getclientaddr(int fd)
{
	struct sockaddr_in addr;
	socklen_t addr_length = sizeof(addr);
	getpeername(fd, (struct sockaddr *)&addr, &addr_length);
	
	return addr;
}

/* converts an address to string */

char * straddr(struct sockaddr_in addr)
{
	char * tmp = inet_ntoa(addr.sin_addr);
	
	return tmp ? tmp : "ADDRESS NOT AVAILABLE"; 
}

/* get client host name */

char * get_client_hostname(int fd)
{
	struct hostent * host;
	struct sockaddr_in addr = getclientaddr(fd);
	
	host = gethostbyaddr(&addr.sin_addr, sizeof(struct in_addr), AF_INET);
	
	return host->h_name;
}

/* get http formatted date stamp
   see : http://tools.ietf.org/html/rfc2616#page-21
*/
/*
  We use days abbrev table to avoid locales
  All days are on 3 characters
 */
static char* days[] = { "Sun", "Mon", "Tue", "Wed", "Thu", "Fri", "Sat" };



static char* months[] = { "Jan", "Feb", "Mar", "Apr",
						   "May", "Jun", "Jul", "Aug",
						   "Sep", "Oct", "Nov", "Dec" };

char * http_date(struct tm *date)
{
	static char date_buf[DATE_BUF_MAX]; /* Declared as static to avoid allocation */
	assert(date);
	sprintf(date_buf, "%s, %02i %s %i %02i:%02i:%02i GMT", days[date->tm_wday], date->tm_mday,
			months[date->tm_mon], date->tm_year+1900,
			date->tm_hour, date->tm_min, date->tm_sec);
	return date_buf;
}
