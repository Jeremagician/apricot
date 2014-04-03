#ifndef _CGI_H_
#define _CGI_H_

void cgi_init();
void cgi_exit();

int cgi_has_post();
int cgi_post_isset(char * key);
char * cgi_post_value(char * key);

int cgi_has_get();
int cgi_get_isset(char * key);
char * cgi_get_value(char * key);



#endif /* _CGI_H_ */
