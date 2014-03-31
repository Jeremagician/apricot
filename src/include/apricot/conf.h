#ifndef _CONF_
#define _CONF_

#include <magic.h>

#define ROOT_MAX 255
#define NR_HOSTS 100
#define HOST_NAME_MAX 255
#define LOG_MAX 255

	typedef struct {
		int listen_port;
		char root[ROOT_MAX];
		char hosts[NR_HOSTS][HOST_NAME_MAX];
		char hosts_root[NR_HOSTS][HOST_NAME_MAX];
		int nr_hosts;
		char log_file[LOG_MAX];
		
		/* libmagic cookie for mime types */
		magic_t magic;
	} conf_t;
	
	/* Lit le fichier de configuration */
	void conf_read(const char * conf_filename);
	
	extern conf_t conf;

#endif
