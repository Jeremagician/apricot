#include <apricot/cookie.h>
#include <apricot/conf.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void cookie_create(char * dest)
{
  char * tmp = tempnam(conf.cookie_root, NULL);
  
  if(tmp == NULL)
	strcpy(dest, "");
  else
	strcpy(dest, tmp);
}