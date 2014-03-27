#include <apricot/conf.h>
#include <apricot/master.h>
#include <apricot/log.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define CONF_FILENAME "apricot.conf"
#define SYSCONF_DIR "/etc/apricot/"

/* Prototypes */
static FILE * open_file();

conf_t conf_read()
{
	conf_t conf;
	FILE * conf_file;
	char buf[255];
	char buf2[255];
	
	conf_file = open_file();
	
	if(!conf_file)
	{
		log_error("Failed to find configuration file apricot.conf");
		exit(EXIT_FAILURE);
	}
	
	conf.listen_port = -1;
	
	while(fgets(buf, 254, conf_file))
	{
		/* pour eviter que des lignes vides soient des erreurs de syntaxe */
		if(*buf == '\n')
			continue;
	
		if(sscanf(buf, "%25s", buf2) != 1)
		{
			log_error("Failed to read configuration file, bad syntax");
			exit(EXIT_FAILURE);
		}
		
		if(!strcmp(buf2, "port"))
		{
			sscanf(buf, "%25s %5u", buf2, &conf.listen_port);
		}
		else
		{
			strcpy(buf, "Unknown option in apricot.conf : ");
			strcat(buf, buf2);
			log_error(buf);
			exit(EXIT_FAILURE);
		}
	}
	
	if(conf.listen_port <= 0 || conf.listen_port >= 65536)
	{
		log_error("Invalid port specified in apricot.conf");
		exit(EXIT_FAILURE);
	}
	
	fclose(conf_file);
	
	return conf;
}

/* Recherche le fichier de configuration apricot.conf dans le répertoire
 courant puis dans le dossier de configuration système /etc/apricot
 si on a été installé avec make install.
 D'autres emplacements de recherche peuvent etre ajoutés dans locations.
  */
static FILE * open_file()
{
	FILE * f;
	int i;
	char * locations[] = { CONF_FILENAME,
						"bin/" CONF_FILENAME,
						SYSCONF_DIR CONF_FILENAME,
						NULL};
	
	f = NULL;
	i = 0;
	
	do
	{
		f = fopen(locations[i], "r");
		i++;
	} while(locations[i] != NULL && !f);		
	
	return f;
}
