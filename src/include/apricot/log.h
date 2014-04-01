#ifndef _LOG_
#define _LOG_

#include <stdarg.h>
#include <stdio.h>

enum
{
	LOG_DEBUG = 1 << 0,
	LOG_INFO = 1 << 1,
	LOG_WARNING = 1 << 2,
	LOG_ERROR = 1 << 3,
	LOG_ALL = ~0
};

void log_info(char * message, ...);
void log_debug(char * message, ...);
void log_warning(char * message, ...);
void log_error(char * message, ...);

void log_set_level(int mask);
void log_set_file(FILE* f);

/* disable and enable the file
 * by locking it */

void log_unlock();
void log_lock();

#endif /* _LOG_ */
