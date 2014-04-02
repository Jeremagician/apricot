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

/* Prototypes */
static void print_help();
static void daemonize();
static void unlock();

int main(int argc, char ** argv)
{
	int port = 0;
	int opt;
	int daemon = 1;
	FILE * log_file;
	char * user_supplied_conf_filename = NULL;

	/* check that apricot is not already running */
	if(open(LOCK_FILE, O_CREAT | O_EXCL) < 0)
	{
		fprintf(stderr, "Apricot is already running.\n");
		exit(EXIT_FAILURE);
	}
	
	atexit(unlock);

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
				daemon = 0;
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

	/* read configuration file */
	conf_read(user_supplied_conf_filename);

	if(daemon)
	{
		daemonize();
		log_file = fopen(conf.log_file, "a+");
	}
	else
		log_file = stdout;


	if(!log_file)
	{
		fprintf(stderr, "Failed to create log file %s\n", conf.log_file);
		exit(EXIT_FAILURE);
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

static void print_help()
{
	printf("\nHelp\n");
	printf("-d : launch the server in debug mode, not as a daemon\n");
	printf("-p <port> : listen port <port>, override settings in apricot.conf\n");
	printf("-c <configuration file> : use the given configuration file\n");
	printf("-v : display the current version of apricot\n");
	printf("-h : display this help\n");
	putchar('\n');
}

static void daemonize()
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
	if(chdir(conf.root) < 0)
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

static void unlock()
{
  unlink(LOCK_FILE);
}