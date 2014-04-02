#include <apricot/cookie.h>
#include <apricot/conf.h>
#include <apricot/log.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

void cookie_create(char * dest)
{
  char buf[255];
  char * tmp;
  int fd;
  
  if(!getcwd(buf, 254))
  {
  	log_error("failed to get current working directory in cookie_create");
  	strcpy(dest, "");
  	return;
  }
  
  strncat(buf, "/cookies", 254);
  
  tmp = tempnam(buf, "c");
  
  if(tmp == NULL)
	strcpy(dest, "");
  else
  {
  	if((fd = creat(tmp, O_RDWR | S_IRUSR | S_IWUSR)) == -1)
  	{
  		strcpy(dest, "");
  		log_error("cookie file creation failed %s", tmp);
  	}
  	else
  	{
  		strcpy(dest, tmp);
  		close(fd);
  	}
  }
}
