#ifndef _MASTER_
#define _MASTER_

void master_start(int port, char ** argv);
void master_stop();

/* arguments of the current process 
 * needed because reincarnate create a new worker
 * and it is asynchronously executed so we cannot
 * transmit argv as an argument to reincarnate */
extern char ** arguments;

#endif
