#ifndef _HTTP_HEADER_
#define _HTTP_HEADER_

typedef struct {
	/* Request line */
	int method;
	int http_version_major;
	int http_version_minor;
	char uri[URI_MAX];
	
	/* Request headers */
	char accept[ACCEPT_MAX];
	char accept-charset[ACCEPT_CHARSET_MAX];
	char accept-encoding[ACCEPT_ENCODING_MAX];
	char accept-language[ACCEPT_LANGUAGE_MAX];
	char authorization[AUTHORIZATION_MAX];
	char expectation[EXPECTATION_MAX];
	char from[FROM_MAX];
	char host[HOST_MAX];
	char referer[URI_MAX];
	char transfer_encoding[TE_MAX];
	char user-agent[USER_AGENT_MAX];
	
	/* Generic headers */
	int cache-control;
	int cache-seconds;
	char connection[CONNECTION_MAX];
	char date[DATE_MAX];
	char pragma[PRAGMA_MAX];
	char trailer[TRAILER_MAX];
	char upgrade[UPGRADE_MAX];
	char via[VIA_MAX];
	int warn_code;
	
} http_request_t;

/* methods */

enum{HTTP_METHOD_OPTIONS,
	HTTP_METHOD_GET,
	HTTP_METHOD_HEAD,
	HTTP_METHOD_POST,
	HTTP_METHOD_PUT,
	HTTP_METHOD_DELETE,
	HTTP_METHOD_TRACE,
	HTTP_METHOD_CONNECT};
	
/* cache directives */

enum{REQUEST_NO_CACHE,
	REQUEST_NO_STORE,
	REQUEST_MAX_AGE,
	REQUEST_MAX_STALE,
	REQUEST_MIN_FRESH,
	REQUEST_NO_TRANSFORM,
	REQUEST_ONLY_IF_CACHED,
	REQUEST_CACHE_EXTENSION
	};
	
#define URI_MAX 2048
#define HOST_MAX 255
#define ACCEPT_MAX 255
#define ACCEPT_CHARSET_MAX 80
#define ACCEPT_ENCODING_MAX 80
#define ACCEPT_LANGUAGE_MAX 80
#define AUTHORIZATION_MAX 80
#define EXPECTATION_MAX 80
#define FROM_MAX 80
#define TE_MAX 80
#define USER_AGENT_MAX 80
#define CONNECTION_MAX 80
#define DATE_MAX 80
#define PRAGMA_MAX 80
#define TRAILER_MAX 80
#define UPGRADE_MAX 80
#define VIA_MAX 80

#endif
