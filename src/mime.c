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
