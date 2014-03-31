#include <apricot/static.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/http_header.h>
#include <apricot/mime.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/mman.h>

int static_serve(int clientfd, char * filename)
{
	static http_response_t response;
	struct stat filestat;
	
	if(stat(filename, &filestat) < 0)
	  return HTTP_NOT_FOUND;

    /* Print the HTTP response */
	http_response_default(&response, 1, 0, HTTP_OK);
	strcpy(response.content_type, mime_type(filename));
	response.content_length = filestat.st_size;
	http_response_write(clientfd, &response);
	
	/* file mapping */
	int filefd = open(filename, O_RDONLY);
	
	if(filefd < 0)
	  return HTTP_NOT_FOUND;
	
	void * file_ptr = mmap(NULL, filestat.st_size, PROT_READ, MAP_PRIVATE, filefd, 0);
	
	if(file_ptr == MAP_FAILED)
	{
		return HTTP_NOT_FOUND;
	}

    Rio_writen(clientfd, file_ptr, filestat.st_size);
	
	munmap(file_ptr, filestat.st_size);
	close(filefd);

	return HTTP_OK;
}
