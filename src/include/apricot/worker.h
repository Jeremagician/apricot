#ifndef _WORKER_H_
#define _WORKER_H_

#include <apricot/csapp.h>
#include <apricot/http_header.h>

/*
  Le module worker sert essentiellement à empêcher l'accès
  à un worker à la pool de processus.

  Les signaux sont redéfinis pour demander à un worker de s'arrêter
  (et de le faire proprement).
 */

void worker_start(int listenfd);

/*
  Chaque worker a une table de cgi associée
 */
#define MAX_CGI 255

typedef struct cgi_t
{
	pid_t pid;
	int clientfd;
    FILE* cgifile;
	http_request_t request;
} cgi_t;

cgi_t cgi_table[MAX_CGI];
extern int cgi_table_size;
void log_request(http_request_t *request, int http_code);

#endif /* _WORKER_H_ */
