#include <apricot/master.h>
#include <apricot/log.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*
 - We need to talk about this
static int debug;

int debug_activated()
{
	return debug;
}
*/

void print_usage(char ** argv)
{
	fprintf(stderr, "usage : %s <port>\n", argv[0]);
}

int main(int argc, char ** argv)
{
	int port;
	int opt;
    
    while((opt = getopt(argc, argv, ":d")) != -1)
	{
		switch(opt)
		{
			case '?':
				fprintf(stderr, "Unknown option %c.\n\n", optopt);
				print_usage(argv);
				exit(EXIT_FAILURE);
			break;
			
			case 'd':
			break;
			
			case ':':
			break;
		}
	}
	
	if (optind == argc) {
		print_usage(argv);
		exit(EXIT_FAILURE);
	}
  
	port = atoi(argv[optind]);
	
	/* start log */
	log_init(stdout); // DEBUG
	
	/* launch master */
	master_start(port);

	return EXIT_SUCCESS;
}
