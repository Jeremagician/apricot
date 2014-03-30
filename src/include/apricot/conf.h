#ifndef _CONF_
#define _CONF_

#define ROOT_MAX 255

	typedef struct {
		int listen_port;
		char root[ROOT_MAX];
	} conf_t;
	
	/* Lit le fichier de configuration */
	void conf_read(const char * conf_filename);
	
	extern conf_t conf;

#endif