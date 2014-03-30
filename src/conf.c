#include <apricot/conf.h>
#include <apricot/master.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#define CONF_FILENAME "apricot.conf"
#define SYSCONF_DIR "/etc/apricot/"

conf_t conf;

/* Prototypes */
static FILE * open_file();

void conf_read(const char * conf_filename)
{
	FILE * conf_file;
	char buf[255];
	char buf2[255];
	struct stat fs;
	
	if(conf_filename)
		conf_file = fopen(conf_filename, "r");
	else
		conf_file = open_file();
	
	if(!conf_file)
	{
		fprintf(stderr, "Failed to find configuration file");
		exit(EXIT_FAILURE);
	}
	
	conf.listen_port = -1;
	conf.root[0] = '\0';
	
	while(fgets(buf, 254, conf_file))
	{
		/* pour eviter que des lignes vides soient des erreurs de syntaxe */
		if(*buf == '\n')
			continue;
	
		if(sscanf(buf, "%25s", buf2) != 1)
		{
			fprintf(stderr, "Failed to read configuration file, bad syntax");
			exit(EXIT_FAILURE);
		}
		
		if(!strcmp(buf2, "port"))
		{
			sscanf(buf, "%25s %5u", buf2, &conf.listen_port);
		}
		else if(!strcmp(buf2, "root"))
		{
			sscanf(buf, "%25s %254s", buf2, conf.root);
		}
	}
	
	if(conf.listen_port <= 0 || conf.listen_port >= 65536)
	{
		fprintf(stderr, "Invalid port specified in apricot.conf\n");
		exit(EXIT_FAILURE);
	}
	
	if(stat(conf.root, &fs) < 0)
	{
		fprintf(stderr, "Invalid root directory specified in apricot.conf\n");
		exit(EXIT_FAILURE);
	}
	
	fclose(conf_file);
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
