#ifndef _POOL_
#define _POOL_

/*
  Création de la pool de processus sur le socket listenfd
 */
void pool_create(int listenfd, int pool_size);

/*
  Destruction de la pool de processus
 */
void pool_destroy();

/*
  Redimension de la pool de processus.

  si il s'agit d'une diminution de taille,
  resize n'impacte pas le traitement des requêtes en cours
 */
void pool_resize(int new_size);

#endif
