#include <apricot/log.h>
#include <stdio.h>
#include <time.h>

static FILE * logfile;

/* Prototypes */
static char * get_date();

/* Log messages */
#define LOG_APRICOT_STOP "Apricot stopped"

void log_init(FILE * log_file)
{
	if(!log_file)
		log_file = stdout;
	else
		logfile = log_file;
}

static char * get_date()
{
	static char buf[255];
	
	time_t now = time(NULL);
	struct tm * now_local = localtime(&now);
	strftime(buf, 255, "%c", now_local);
	
	return buf;
}

void log_apricot_stop()
{
	fprintf(logfile, "[%s] %s\n", get_date(), LOG_APRICOT_STOP);
}