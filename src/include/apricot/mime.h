#ifndef _MIME_
#define _MIME_

/* returns the mime type of a file given its filename */
const char * mime_type(char * filename);

/* open mime types library */
int mime_init();

/* close mime library */
void mime_close();

#endif
