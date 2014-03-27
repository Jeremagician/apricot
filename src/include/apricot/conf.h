#ifndef _CONF_
#define _CONF_

	typedef struct {
		int listen_port;
	} conf_t;
	
	/* Lit le fichier de configuration */
	conf_t conf_read(const char * conf_filename);

#endif