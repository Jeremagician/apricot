#ifndef _POOL_
#define _POOL_

/*
  Création de la pool de processus sur le socket listenfd
 */
void pool_create(int listenfd);

/*
  Destruction de la pool de processus
 */
void pool_destroy();

#endif
