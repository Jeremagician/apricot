#ifndef _WORKER_H_
#define _WORKER_H_

/*
  Le module worker sert essentiellement à empêcher l'accès
  à un worker à la pool de processus.

  Les signaux sont redéfinis pour demander à un worker de s'arrêter
  (et de le faire proprement).
 */

void worker_start(int listenfd);

#endif /* _WORKER_H_ */
