#ifndef _CGI_H_
#define _CGI_H_

#define COOKIE_KEY_MAX 512
#define COOKIE_VALUE_MAX 8192

void cgi_init();
void cgi_exit();

int cgi_has_post();
int cgi_post_isset(char * key);
char * cgi_post_value(char * key);

int cgi_has_get();
int cgi_get_isset(char * key);
char * cgi_get_value(char * key);

int cgi_has_cookie();
int cgi_cookie_isset(char * key);
char * cgi_cookie_get(char * key);
void cgi_cookie_set(char * key, char * value);

#endif /* _CGI_H_ */
