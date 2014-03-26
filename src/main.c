#include <apricot/master.h>
#include <stdlib.h>

static int debug;

void debug_activated()
{
	return debug;
}

int main(int argc, char ** argv)
{
	/* launch master */
	master_start();

	return EXIT_SUCCESS;
}
