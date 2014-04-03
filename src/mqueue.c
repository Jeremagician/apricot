#include <apricot/mqueue.h>
#include <apricot/master.h>
#include <apricot/log.h>
#include <apricot/conf.h>
#include <apricot/pool.h>

#include <mqueue.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

/* Message queues
 * Must be static to this file to be unlinked in master_destroy */
static mqd_t from_client = -1;
static mqd_t to_client = -1;

static void send(mqd_t mq, char * msg);

void mqueue_start()
{
	char message_buffer[MQUEUE_BUFFER_SIZE];
	
	from_client = mq_open(MQUEUE_SERVER, O_RDONLY | O_EXCL | O_CREAT, S_IRUSR | S_IWUSR, NULL);
	to_client = mq_open(MQUEUE_CLIENT, O_WRONLY | O_EXCL | O_CREAT, S_IWUSR | S_IRUSR, NULL);
	
	if(from_client == -1 || to_client == -1)
	{
	  log_error("Failed to open administration interface, mq_open failed");
	  master_stop();
	}
	
	log_info("Apricot admin interface opened");
	  
	for(;;)
	{
		/* receive a message from the client administration interface */
		if(mq_receive(from_client, message_buffer, MQUEUE_BUFFER_SIZE, NULL) == -1)
		{
		  log_error("mq_receive failed : %s", strerror(errno));
		  continue;
		}
		
		/* process interface request */
		
		if(!strcasecmp(message_buffer, MQUEUE_SEND_LOG))
		{
			FILE * log_file = log_file = fopen(conf.log_file, "r");
			
			if(!log_file)
			{
			  strcpy(message_buffer, MQUEUE_FAIL);
			  send(to_client, message_buffer);
			}
			else
			{
			  while(fgets(message_buffer, MQUEUE_BUFFER_SIZE-1, log_file))
			  {
				send(to_client, message_buffer);
			  }
			  
			  fclose(log_file);
			  strcpy(message_buffer, MQUEUE_OK);
			  send(to_client, message_buffer);
			}
		}
		else if(!strcasecmp(message_buffer, MQUEUE_TRUNCATE_LOG))
		{
			/* to preserve log consistency */
			log_lock();
			truncate(conf.log_file, 0);
			log_unlock();
			
			strcpy(message_buffer, MQUEUE_OK);
			send(to_client, message_buffer);
		}
		else if(!strcasecmp(message_buffer, MQUEUE_STOP_SERVER))
		{	
			log_info("Apricot stop asked by admin interface");
			master_stop();
			exit(EXIT_SUCCESS);
		}
		else if(!strcasecmp(message_buffer, MQUEUE_RESIZE_POOL))
		{
			log_info("Pool resize asked by admin interface");
			
			/* receive a message from the client administration interface */
			if(mq_receive(from_client, message_buffer, MQUEUE_BUFFER_SIZE, NULL) == -1)
			{
			  log_error("mq_receive failed to receive new pool size : %s", strerror(errno));
			  continue;
			}
			
			int new_size;
			
			if(sscanf(message_buffer, "%i", &new_size) != 1 || new_size <= 0)
			{
				log_error("invalid pool size %i received from admin interface", new_size);
				strcpy(message_buffer, MQUEUE_FAIL);
				send(to_client, message_buffer);
				continue;
			}
			
			pool_resize(new_size);
			
			strcpy(message_buffer, MQUEUE_OK);
			send(to_client, message_buffer);
		}
		else
		{
		   	strcpy(message_buffer, MQUEUE_FAIL);
		   	send(to_client, message_buffer);
		}
	}
}

void mqueue_stop()
{
	if(from_client != -1)
	{
	  mq_close(from_client);
	  mq_unlink(MQUEUE_SERVER);
	}
	
	if(to_client != -1)
	{
	  mq_close(to_client);
	  mq_unlink(MQUEUE_CLIENT);
	}
}

void send(mqd_t mq, char * msg)
{
  if(mq_send(mq, msg, MQUEUE_BUFFER_SIZE, 1) == -1)
  {
	log_error("mq_send failed : %s", strerror(errno));
		  
	/* if we are interrupted by a signal handler we retry to send the response */
	while(mq_send(to_client, msg, MQUEUE_BUFFER_SIZE, 1) == -1 && errno == EINTR);
  }
}
