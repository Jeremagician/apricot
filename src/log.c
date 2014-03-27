#include <apricot/log.h>
#include <stdio.h>
#include <time.h>
#include <assert.h>
#include <time.h>
#include <stdlib.h>

static void vlog(char * tag, char * message, va_list ap);

static FILE* log_file = NULL;
static int log_level = 0;

void log_set_level(int mask)
{
	log_level = mask;
}

void log_set_file(FILE* f)
{
	log_file = f;
}

void log_info(char * message, ...)
{
	if((log_level & LOG_INFO) != 0)
	{
		va_list ap;
		va_start(ap, message);
		vlog("INFO", message, ap);
		va_end(ap);
	}
}

void log_debug(char * message, ...)
{
	if((log_level & LOG_DEBUG) != 0)
	{
		va_list ap;
		va_start(ap, message);
		vlog("DEBUG", message, ap);
		va_end(ap);
	}
}

void log_warning(char * message, ...)
{
	if((log_level & LOG_WARNING) != 0)
	{
		va_list ap;
		va_start(ap, message);
		vlog("WARNING", message, ap);
		va_end(ap);
	}
}

void log_error(char * message, ...)
{
	if((log_level & LOG_ERROR) != 0)
	{
		va_list ap;
		va_start(ap, message);
		vlog("ERROR", message, ap);
		va_end(ap);
	}
}

static void vlog(char * tag, char * message, va_list ap)
{
	time_t now;
	char buf_time[255];

	time(&now);
	strftime(buf_time, 255, "%Y-%m-%d %02k:%02M", localtime(&now));

	fprintf(log_file, "[%s] %s : ", buf_time, tag);
	vfprintf(log_file, message, ap);
	fprintf(log_file, "\n");
	
	/* nécessaire pour que les logs soient envoyés en continu dans le
	fichier quand on est un daemon */
	fflush(log_file);
}


