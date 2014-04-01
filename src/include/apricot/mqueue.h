#ifndef _MQUEUE_
#define _MQUEUE_

#define MQUEUE_SERVER "/apricot_server"
#define MQUEUE_CLIENT "/apricot_admin"
#define MQUEUE_BUFFER_SIZE 8192

/* messages */
#define MQUEUE_OK "OK"
#define MQUEUE_FAIL "FAIL"
#define MQUEUE_SEND_LOG "LOGSEND"
#define MQUEUE_TRUNCATE_LOG "TRUNCATELOG"
#define MQUEUE_STOP_SERVER "STOP"

/* start message queue for administration interface */
void mqueue_start();

/* stop message queue, must be called in master_stop */
void mqueue_stop();

#endif