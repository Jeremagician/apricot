#ifndef _MASTER_
#define _MASTER_

#define LOCK_FILE "/tmp/.apricot.lock"

void master_start(int port, char ** argv);
void master_stop();

/* Arguments of the current process 
 * needed because reincarnate create a new worker
 * and it is asynchronously executed so we cannot
 * transmit argv as an argument to reincarnate */
extern char ** arguments;

#endif
