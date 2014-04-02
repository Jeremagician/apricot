#include <apricot/mime.h>
#include <apricot/conf.h>
#include <apricot/log.h>
#include <apricot/magic.h>

#include <string.h>
#include <stdlib.h>

const char * mime_type(char * filename)
{
	const char * type = magic_file(conf.magic, filename);
	
	if(!type)
	{
		log_info("libmagic magic_file failed for file %s", filename);
		log_info("libmagic %s", magic_error(conf.magic));
		return "text/html";
	}
	
	return type;
}

/* open magic library */
int mime_init()
{
	if(!(conf.magic = magic_open(MAGIC_MIME_TYPE)))
	{
	  log_error("failed to open libmagic");
	  return -1;
	}
	
	if(magic_load(conf.magic, NULL) == -1)
		return -1;
		
	return 0;
}

void mime_close()
{
	magic_close(conf.magic);
}
