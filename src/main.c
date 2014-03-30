#include <apricot/master.h>
#include <apricot/log.h>
#include <apricot/conf.h>

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

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
	char * user_supplied_conf_filename = NULL;
	
	/* check that apricot is not already running */
	if(open(LOCK_FILE, O_CREAT | O_EXCL) < 0)
	{
		fprintf(stderr, "Apricot is already running.\n");
		exit(EXIT_FAILURE);
	}
    
    while((opt = getopt(argc, argv, "dp:vhc:")) != -1)
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
			
			case 'c':
				user_supplied_conf_filename = optarg;
			break;
				
			case ':':
				fprintf(stderr, "Invalid option\n");
				print_help();
				exit(EXIT_FAILURE);
			break;
		}
	}
	
	if(optind != argc)
	{
		fprintf(stderr, "Bad option\n");
		print_help();
		exit(EXIT_FAILURE);
	}
	
	/* read configuration file 
		Affiche les erreurs sur l'entrée standard au lieu du fichier de log
		car c'est mieux de ne pas lancer un daemon s'il n'a pas réussit à lire
		sa configuration.
	*/
	conf_read(user_supplied_conf_filename);
	
	if(daemon)
	{
		daemonize();
		
		/* Si on est un démon on doit écrire les logs dans un fichier
		disponible sur l'ensemble du système. On utilise pour le
		moment /tmp/apricot.log */
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
	master_start(port, argv);

	return EXIT_SUCCESS;
}

void print_help()
{
	printf("\nHelp\n");
	printf("-d : launch the server as a daemon, logs are written in /tmp/apricot.log\n");
	printf("-p <port> : listen port <port>, override settings in apricot.conf\n");
	printf("-c <configuration file> : use the given configuration file\n");
	printf("-v : display the current version of apricot\n");
	printf("-h : display this help\n");
	putchar('\n');
}

void daemonize()
{
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
	
	/* On vérifie que notre père est bien init */
	if(getppid() != 1)
	{
		fprintf(stderr, "Failed to daemonize, not child process of init\n");
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
