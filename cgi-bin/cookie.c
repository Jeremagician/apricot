#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>

int main()
{ 
  char * cookie_filename;
  char buf[255];
  
  printf("Content-type: text/html\n");
  
  if((cookie_filename = getenv("COOKIE")) != NULL)
  {
	printf("<html><body><p>File %s</p>\n", cookie_filename);
	
	FILE * cookie_file = fopen(cookie_filename, "w");
	
	if(!cookie_file)
	{
		return EXIT_FAILURE;
	}
	
	fclose(cookie_file);	
  }
  else
  {
	printf("<html><body><h1>Erreur</h1></body></html>\n");
  }
  
  fflush(stdout);
  
  return EXIT_SUCCESS;
}
