#include "cgi.h"
#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n1=0, n2=0;

	printf("Content-type: text/plain\n");

	cgi_init();

    printf("Welcome to the Internet addition portal.\n");

	if(cgi_has_get() && cgi_get_isset("arg1") && cgi_get_isset("arg2"))
	{
		n1 = atoi(cgi_get_value("arg1"));
		n2 = atoi(cgi_get_value("arg2"));
	}

	printf("The answer is: %d + %d = %d\n", n1, n2, n1 + n2);
	printf("Thanks for visiting!\n");

    fflush(stdout);

	cgi_exit();

    exit(EXIT_SUCCESS);
}
