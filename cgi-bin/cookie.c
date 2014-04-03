#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include "cgi.h"

int main()
{ 
  char * cookie_filename;
  char buf[255];
  int cpt;
  
  printf("Content-type: text/html\n");
  
  cgi_init();
  
  printf("<html><head><title>Cookies</title></head><body>\n");
  printf("<h1>Cookie</h1>\n");
  
  if(cgi_cookie_isset("cpt"))
  {
  	cpt = atoi(cgi_cookie_get("cpt"));
  	cpt++;
  }
  else
  {
  	cpt = 1;
  }
  
  if(cgi_get_isset("message"))
  {
  	strcpy(buf, cgi_get_value("message"));
  	cgi_cookie_set("message", buf);
  }
  else
  {
  	if(cgi_cookie_isset("message"))
  	{
  		strcpy(buf, cgi_cookie_get("message"));
  	}
  	else
  	{
  		strcpy(buf, "aucun message");
  	}
  }
  
  printf("<p>Ceci est votre %i eme visite.</p>\n", cpt);
  printf("<p>Message : %s</p>\n", buf);
  
  if(!cgi_cookie_isset("data"))
  {
  	cgi_cookie_set("data", "Ceci est une ligne avec un\n backslash n");
  }
  
  printf("<p>%s</p>", cgi_cookie_get("data"));
  
  printf("</body></html>\n");
  
  sprintf(buf, "%i", cpt);
  cgi_cookie_set("cpt", buf);
  
  cgi_exit();
  
  fflush(stdout);
  
  return EXIT_SUCCESS;
}
