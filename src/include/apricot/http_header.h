#ifndef _HTTP_HEADER_
#define _HTTP_HEADER_

#include <time.h>
#include <netinet/in.h>

#define URI_MAX 8096
#define METHOD_MAX 25
#define HOST_MAX 255
#define ACCEPT_MAX 255
#define ACCEPT_CHARSET_MAX 80
#define ACCEPT_ENCODING_MAX 80
#define ACCEPT_LANGUAGE_MAX 80
#define ACCEPT_RANGES_MAX 80
#define AUTHORIZATION_MAX 80
#define CACHE_CONTROL_MAX 255
#define CONTENT_ENCODING_MAX 25
#define CONTENT_LANGUAGE_MAX 80
#define CONTENT_MD5_MAX 255
#define CONTENT_RANGE_MAX 80
#define CONTENT_TYPE_MAX 80
#define COOKIE_ID_MAX 255
#define ETAG_MAX 80
#define EXPECTATION_MAX 80
#define FROM_MAX 80
#define TE_MAX 80
#define USER_AGENT_MAX 80
#define CONNECTION_MAX 80
#define DATE_MAX 80
#define PRAGMA_MAX 80
#define PROXY_AUTH_MAX 80
#define REASON_PHRASE_MAX 80
#define RETRY_AFTER_MAX 80
#define SERVER_MAX 80
#define TRAILER_MAX 80
#define UPGRADE_MAX 80
#define VARY_MAX 80
#define VIA_MAX 80
#define WWW_AUTH_MAX 80

/* Contains the header of a request message
from http://www.w3.org/Protocols/rfc2616/rfc2616.html */

typedef struct {
	/* client address */
	struct sockaddr_in client_address;

	/* Request line */
	int method;
	int http_version_major;
	int http_version_minor;
	char uri[URI_MAX];

	/* Request headers */
	char accept[ACCEPT_MAX];
	char accept_charset[ACCEPT_CHARSET_MAX];
	char accept_encoding[ACCEPT_ENCODING_MAX];
	char accept_language[ACCEPT_LANGUAGE_MAX];
	char authorization[AUTHORIZATION_MAX];
	char expect[EXPECTATION_MAX];
	char from[FROM_MAX];
	char host[HOST_MAX];
	char referer[URI_MAX];
	char transfer_encoding[TE_MAX];
	char user_agent[USER_AGENT_MAX];

	/* General headers */
	char cache_control[CACHE_CONTROL_MAX];
	char connection[CONNECTION_MAX];
	struct tm date;
	char pragma[PRAGMA_MAX];
	char trailer[TRAILER_MAX];
	char upgrade[UPGRADE_MAX];
	char via[VIA_MAX];
	int warn_code;

	/* entity headers */
	int allow;
	char content_encoding[CONTENT_ENCODING_MAX];
	char content_language[CONTENT_LANGUAGE_MAX];
	int content_length;
	char content_location[URI_MAX];
	char content_md5[CONTENT_MD5_MAX];
	char content_range[CONTENT_RANGE_MAX];
	char content_type[CONTENT_TYPE_MAX];
	struct tm expires;
	struct tm last_modified;
	
	char cookie_id[COOKIE_ID_MAX];

} http_request_t;

typedef struct {
	/* Response line */
	int http_version_major;
	int http_version_minor;
	int status_code;
	char reason_phrase[REASON_PHRASE_MAX];

	/* Response headers */
	char accept_ranges[ACCEPT_RANGES_MAX];
	int age;
	char etag[ETAG_MAX];
	char location[URI_MAX];
	char proxy_authenticate[PROXY_AUTH_MAX];
	char retry_after[RETRY_AFTER_MAX];
	char server[SERVER_MAX];
	char vary[VARY_MAX];
	char www_authenticate[WWW_AUTH_MAX];

	/* General headers */
	char cache_control[CACHE_CONTROL_MAX];
	char connection[CONNECTION_MAX];
	struct tm date;
	char pragma[PRAGMA_MAX];
	char trailer[TRAILER_MAX];
	char upgrade[UPGRADE_MAX];
	char via[VIA_MAX];
	int warn_code;

	/* entity headers */
	int allow;
	char content_encoding[CONTENT_ENCODING_MAX];
	char content_language[CONTENT_LANGUAGE_MAX];
	int content_length;
	char content_location[URI_MAX];
	char content_md5[CONTENT_MD5_MAX];
	char content_range[CONTENT_RANGE_MAX];
	char content_type[CONTENT_TYPE_MAX];
	struct tm expires;
	struct tm last_modified;

} http_response_t;

/* methods */

#define HTTP_METHOD_STR(x) (\
		(x==HTTP_METHOD_OPTIONS) ? "OPTIONS" :		\
		(x==HTTP_METHOD_GET) ? "GET" :				\
		(x==HTTP_METHOD_HEAD) ? "HEAD" :			\
		(x==HTTP_METHOD_POST) ? "POST" :			\
		(x==HTTP_METHOD_PUT) ? "PUT" :				\
		(x==HTTP_METHOD_DELETE) ? "DELETE" :		\
		(x==HTTP_METHOD_TRACE) ? "TRACE" :			\
		(x==HTTP_METHOD_CONNECT) ? "CONNECT" :		\
		"invalid" )

enum{HTTP_METHOD_OPTIONS = 1,
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

/* allow */
enum{	ALLOW_GET,
	ALLOW_HEAD,
	ALLOW_PUT
    };

/* fills the request header according to the data read on fd */

int http_request_read(int fd, http_request_t * request);
void http_response_write(int fd, http_response_t *response);

/*
  Init an http_response to default values
 */
void http_response_default(http_response_t *response, int http_major, int http_minor, int status);

/* request headers fields names hash values according to djb2 function
 * defined in utils.c */

#define HASH_ACCEPT 864
#define HASH_ACCEPT_CHARSET 839
#define HASH_ACCEPT_ENCODING 436
#define HASH_ACCEPT_LANGUAGE 81
#define HASH_AUTHORIZATION 737
#define HASH_EXCEPT 777
#define HASH_FROM 492
#define HASH_HOST 238
#define HASH_IF_MATCH 473
#define HASH_IF_MODIFIED_SINCE 220
#define HASH_IF_NONE_MATCH 78
#define HASH_IF_RANGE 673
#define HASH_UNMODIFIED_SINCE 919
#define HASH_MAX_FORWARDS 635
#define HASH_PROXY_AUTHORIZATION 626
#define HASH_RANGE 173
#define HASH_REFERER 971
#define HASH_TE 929
#define HASH_USER_AGENT 219
#define HASH_CACHE_CONTROL 474
#define HASH_CONNECTION 512
#define HASH_DATE 262
#define HASH_PRAGMA 464
#define HASH_TRAILER 627
#define HASH_TRANSFER_ENCODING 169
#define HASH_UPGRADE 448
#define HASH_VIA 64
#define HASH_WARNING 918
#define HASH_ALLOW 927
#define HASH_CONTENT_ENCODING 295
#define HASH_CONTENT_LANGUAGE 236
#define HASH_CONTENT_LENGTH 714
#define HASH_CONTENT_LOCATION 817
#define HASH_CONTENT_MD5 22
#define HASH_CONTENT_RANGE 669
#define HASH_CONTENT_TYPE 210
#define HASH_EXPIRES 984
#define HASH_LAST_MODIFIED 634
#define HASH_COOKIE 234

#endif
