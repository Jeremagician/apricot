#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include "cgi.h"
#include "../src/include/apricot/mqueue.h"

#define PRINT_LOG_ACTION "printlog"
#define TRUNCATE_LOG_ACTION "truncatelog"
#define SERVER_STOP_ACTION "serverstop"
#define POOL_RESIZE_ACTION "resizepool"

static void send(mqd_t mq, char * msg);
static void receive(mqd_t mq, char * msg);

int main()
{
  char * buf;
  char action[80];
  char msg[MQUEUE_BUFFER_SIZE];
  mqd_t to_server;
  mqd_t from_server;
  
  cgi_init();

  to_server = mq_open(MQUEUE_SERVER, O_WRONLY);
  from_server = mq_open(MQUEUE_CLIENT, O_RDONLY);

  if(to_server == -1 || from_server == -1)
	return EXIT_FAILURE;

  printf("Content-type: text/html\n");

  if (cgi_has_get() && cgi_get_isset("action"))
  {
  	strncpy(action, cgi_get_value("action"), 79);
  	
	if(!strcmp(action, PRINT_LOG_ACTION))
	{
	  printf("<html><head><title>Apricot Admin Interface</title>\n");
	  printf("</head>\n\n");
	  printf("<body>\n");

	  strcpy(msg, MQUEUE_SEND_LOG);
	  send(to_server, msg);

	  printf("<h1>Apricot server log</h1>\n");

	  receive(from_server, msg);

	  while(strcmp(msg, MQUEUE_OK) && strcmp(msg, MQUEUE_FAIL))
	  {
		printf("<p>%s</p>", msg);
		receive(from_server, msg);
	  }

	  printf("</body>\n");
	  printf("</html>\n");
	}
	else if(!strcmp(action, TRUNCATE_LOG_ACTION))
	{
	  printf("<html><head><title>Apricot Admin Interface</title>\n");
	  printf("</head>\n\n");
	  printf("<body>\n");

	  strcpy(msg, MQUEUE_TRUNCATE_LOG);
	  send(to_server, msg);
	  receive(from_server, msg);

	  if(!strcmp(msg, MQUEUE_OK))
	  {
		printf("<h1>Log file reset done</h1>\n");
	  }
	  else
	  {
		printf("<h1>Log file reset failed</h1>\n");
	  }

	  printf("</body>\n");
	  printf("</html>\n");
	}
	else if(!strcmp(action, SERVER_STOP_ACTION))
	{
	  printf("<html><head><title>Apricot Admin Interface</title>\n");
	  printf("</head>\n\n");
	  printf("<body>\n");

	  strcpy(msg, MQUEUE_STOP_SERVER);
	  send(to_server, msg);
	  
	  printf("<h1 style=\"color:red;\"> Server stopped</h1>\n");
	  printf("</body>\n");
	  printf("</html>\n");
	}
	else if(!strcmp(action, POOL_RESIZE_ACTION))
	{
		  printf("<html><head><title>Apricot Admin Interface</title>\n");
		  printf("</head>\n\n");
		  printf("<body>\n");
		  
		  if(!cgi_get_isset("size"))
		  {
		  	printf("<form method=\"get\" action=\"admin\">\n");
		  	printf("<input type=\"hidden\" name=\"action\" value=\"resizepool\"></input>\n");
		  	printf("<label>Pool size : </label><input type=\"text\" size=\"20\" name=\"size\"></input>\n");
		  	printf("<input type=\"submit\" value=\"Ok\"></input></form>\n");
		  }
		  else
		  {
		  	strcpy(msg, MQUEUE_RESIZE_POOL);
		  	send(to_server, msg);
		  	strcpy(msg, cgi_get_value("size"));
		  	send(to_server, msg);
		  	receive(from_server, msg);

			if(!strcmp(msg, MQUEUE_OK))
			{
				printf("<h1>Pool resize done</h1>\n");
			}
			else
			{
				printf("<h1>Pool resize failed</h1>\n");
			}
		  }
		  
		  printf("</body>\n");
		  printf("</html>\n");
	}
	else
	{
	  printf("<html><head><title>Apricot Admin Interface</title>\n");
	  printf("</head>\n\n");
	  printf("<body>\n");
	  printf("<h1>Unknown action %s</h1>\n", action);
	  printf("</body>\n");
	  printf("</html>\n");
	}
  }
  else
  {
	printf("<html><head><title>Apricot Admin Interface</title></head>\n\n");
	printf("<body>\n");
	printf("<h1>Apricot administration interface</h1>\n");
	printf("</br><a href=\"?action=printlog\">Print the log file</a>\n");
	printf("</br><a href=\"?action=truncatelog\">Reset log file</a>\n");
	printf("</br><a href=\"?action=serverstop\">Stop server</a>\n");
	printf("</br><a href=\"?action=resizepool\">Resize workers pool</a>\n");
	printf("</body>\n");
	printf("</html>\n");
  }

	cgi_exit();

  fflush(stdout);
  return EXIT_SUCCESS;
}

void send(mqd_t mq, char * msg)
{
  if(mq_send(mq, msg, MQUEUE_BUFFER_SIZE, 1) == -1)
  {
	exit(EXIT_FAILURE);
  }
}

void receive(mqd_t mq, char * msg)
{
  if(mq_receive(mq, msg, MQUEUE_BUFFER_SIZE, NULL) == -1)
  {
	exit(EXIT_FAILURE);
  }
}
