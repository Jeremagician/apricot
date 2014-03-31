#include <apricot/mime.h>
#include <apricot/conf.h>
#include <apricot/log.h>
#include <string.h>
#include <stdlib.h>
#include <magic.h>

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
