#include <apricot/master.h>
#include <apricot/log.h>
#include <apricot/conf.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

#define APRICOT_VERSION 0.1

/* Prototypes */
void print_help();

int main(int argc, char ** argv)
{
	int port = 0;
	int opt;
    
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
	
	/* start log */
	log_set_level(LOG_ALL);
	log_set_file(stdout);
	
	/* read configuration file */
	conf_t conf = conf_read();

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
