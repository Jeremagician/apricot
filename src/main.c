#include <apricot/master.h>
#include <stdlib.h>
#include <stdio.h>

/*
 - We need to talk about this
static int debug;

int debug_activated()
{
	return debug;
}
*/
int main(int argc, char ** argv)
{
	int port;
	if (argc != 2) {
        fprintf(stderr, "usage: %s <port>\n", argv[0]);
        exit(1);
    }
	port = atoi(argv[1]);
	
	/* launch master */
	master_start(port);

	return EXIT_SUCCESS;
}
