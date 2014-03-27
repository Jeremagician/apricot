#include <apricot/master.h>
#include <apricot/log.h>
#include <apricot/conf.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>

#define APRICOT_VERSION 0.1
#define DEFAULT_DAEMON_LOGFILE "/tmp/apricot.log"

/* Prototypes */
void print_help();
void daemonize();

int main(int argc, char ** argv)
{
	int port = 0;
	int opt;
	int daemon = 0;
	FILE * log_file;
    
    while((opt = getopt(argc, argv, "dp:vh")) != -1)
	{
		switch(opt)
		{
			case '?':
				fprintf(stderr, "Unknown option %c.\n\n", optopt);
				print_help();
				exit(EXIT_FAILURE);
			break;
			
			case 'd':
				daemon = 1;
			break;
			
			case 'p':
				port = atoi(optarg);
				
				if(port <= 0 || port >= 65536)
				{
					fprintf(stderr, "Invalid port %i\n", port);
					print_help();
					exit(EXIT_FAILURE);
				}
			break;
			
			case 'v':
				printf("Apricot Web Server - version %.2f\n", APRICOT_VERSION);
				exit(EXIT_FAILURE);
				break;
			break;
			
			case 'h':
				print_help();
				exit(EXIT_FAILURE);
			break;
				
			case ':':
				fprintf(stderr, "Invalid option\n");
				print_help();
				exit(EXIT_FAILURE);
			break;
		}
	}
	
	/* read configuration file */
	conf_t conf = conf_read();
	
	if(daemon)
	{
		daemonize();
		
		/* if we are a daemon we have to write logs to
		a generic system file until we can do better, like
		syslog for example. This behavior (which file) will be set
		in apricot.conf */
		log_file = fopen(DEFAULT_DAEMON_LOGFILE, "a+");
	}
	else
	{
		log_file = stdout;
	}
	
	/* start log */
	log_set_level(LOG_ALL);
	log_set_file(log_file);

	if(!port)
		port = conf.listen_port;

	/* launch master */
	master_start(port);

	return EXIT_SUCCESS;
}

void print_help()
{
	printf("\nHelp\n");
	printf("-d : launch the server as a daemon\n");
	printf("-p <port> : listen port <port>, override settings in apricot.conf\n");
	printf("-v : display the current version of apricot\n");
	putchar('\n');
}

void daemonize()
{
	/* On verifie que l'on a pas déja une instance démarrée */
	

	pid_t pid = fork();
		
	if(pid == -1)
	{
		fprintf(stderr, "Failed to daemonize, fork failed\n");
		exit(EXIT_FAILURE);
	}
		
	if(pid > 0) // le pere se termine
		exit(EXIT_SUCCESS);
		
	/* On crée une nouvelle session dont on devient le leader.
	On est à présent détaché du terminal de contrôle */
	if(setsid() == -1)
	{
		fprintf(stderr, "Failed to daemonize, setsid failed\n");
		exit(EXIT_FAILURE);
	}
	
	/* On doit changer de répertoire courant pour ne plus dépendre
	d'un répertoire utilisateur, on choisit la racine pour le moment */
	if(chdir("/") < 0)
	{
		fprintf(stderr, "Failed to daemonize, chdir failed\n");
		exit(EXIT_FAILURE);
	}
	
	/* On change l'umask */
	umask(0);
	
	/* on ferme les fichiers standards pour ne plus dépendre du
	terminal de contrôle */
	fclose(stderr);
	fclose(stdout);
	fclose(stdin);
}
