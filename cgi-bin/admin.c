#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <mqueue.h>
#include "../src/include/apricot/mqueue.h"

#define PRINT_LOG_ACTION "printlog"
#define TRUNCATE_LOG_ACTION "truncatelog"
#define SERVER_STOP_ACTION "serverstop"

static void send(mqd_t mq, char * msg);
static void receive(mqd_t mq, char * msg);

int main()
{
  char * buf;
  char action[80];
  char msg[MQUEUE_BUFFER_SIZE];
  mqd_t to_server;
  mqd_t from_server;
  
  to_server = mq_open(MQUEUE_SERVER, O_WRONLY);
  from_server = mq_open(MQUEUE_CLIENT, O_RDONLY);
  
  if(to_server == -1 || from_server == -1)
	return EXIT_FAILURE;
  
  if ((buf = getenv("QUERY_STRING")) != NULL && *buf && sscanf(buf, "action=%79s", action) == 1)
  {
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
	printf("?action=printlog  print the log file\n");
	printf("?action=truncatelog	 reset log file\n");
	printf("?action=serverstop	 stop server\n");
	printf("</body>\n");
	printf("</html>\n");
  }
  
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