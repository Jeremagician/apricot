#define _XOPEN_SOURCE
#include <apricot/http_header.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/http_error.h>
#include <apricot/log.h>
#include <apricot/utils.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE METHOD_MAX + URI_MAX + 10
#define TABLE_SIZE 1000

#define REPONSE_LINE_SIZE (REASON_PHRASE_MAX + 10)
#define FIELD_MAX 255
#define SERVER_NAME "Apricot Web Server"

/* buffer to read a line through the connection */
static char buf[BUF_SIZE];

int http_request_read(int fd, http_request_t * request)
{
	rio_t rio;
	char method[METHOD_MAX];
	char * field;
	char * content;
	static void * request_header_table[TABLE_SIZE] = {
		[HASH_ACCEPT] = &&accept,
		[HASH_ACCEPT_CHARSET] = &&accept_charset,
		[HASH_ACCEPT_ENCODING] = &&accept_encoding,
		[HASH_ACCEPT_LANGUAGE] = &&accept_language,
		[HASH_AUTHORIZATION] = &&authorization,
		[HASH_EXCEPT] = &&expect,
		[HASH_FROM] = &&from,
		[HASH_HOST] = &&host,
		[HASH_IF_MATCH] = &&not_supported,
		[HASH_IF_MODIFIED_SINCE] = &&not_supported,
		[HASH_IF_NONE_MATCH] = &&not_supported,
		[HASH_RANGE] = &&not_supported,
		[HASH_UNMODIFIED_SINCE] = &&not_supported,
		[HASH_MAX_FORWARDS] = &&not_supported,
		[HASH_PROXY_AUTHORIZATION] = &&not_supported,
		[HASH_RANGE] = &&not_supported,
		[HASH_REFERER] = &&referer,
		[HASH_TE] = &&transfer_encoding,
		[HASH_USER_AGENT] = &&user_agent,
		[HASH_CACHE_CONTROL] = &&cache_control,
		[HASH_CONNECTION] = &&connection,
		[HASH_DATE] = &&date,
		[HASH_PRAGMA] = &&pragma,
		[HASH_TRAILER] = &&trailer,
		[HASH_TRANSFER_ENCODING] = &&transfer_encoding,
		[HASH_UPGRADE] = &&upgrade,
		[HASH_VIA] = &&via,
		[HASH_WARNING] = &&warning,
		[HASH_ALLOW] = &&allow,
		[HASH_CONTENT_ENCODING] = &&content_encoding,
		[HASH_CONTENT_LANGUAGE] = &&content_language,
		[HASH_CONTENT_LENGTH] = &&content_length,
		[HASH_CONTENT_LOCATION] = &&content_location,
		[HASH_CONTENT_MD5] = &&content_md5,
		[HASH_CONTENT_RANGE] = &&content_range,
		[HASH_CONTENT_TYPE] = &&content_type,
		[HASH_EXPIRES] = &&expires,
		[HASH_LAST_MODIFIED] = &&last_modified,
		&&not_supported
	};

	Rio_readinitb(&rio, fd);

	/* read request line */
	Rio_readlineb(&rio, buf, BUF_SIZE);

	if(sscanf(buf, "%24s %8096s HTTP/%i.%i", method, request->uri, &request->http_version_major, &request->http_version_minor) != 4)
	{
		 http_clienterror(fd, HTTP_BAD_REQUEST, "Bad request line");
		 return -1;
	}

	/* method */
	if(strcasecmp(method, "GET"))
	{
		http_clienterror(fd, HTTP_NOT_IMPLEMENTED, "Apricot does not implement his method");
		return -1;
	}

	request->method = HTTP_METHOD_GET;

	/* check http version number */
	if(request->http_version_major < 0 || request->http_version_minor < 0
		|| request->http_version_major > 9 || request->http_version_minor > 9)
	{
		http_clienterror(fd, HTTP_BAD_REQUEST, "Bad HTTP version");
		return -1;
	}

	/* read client IP address */
	request->client_address = getclientaddr(fd);

	/* read other headers */
	Rio_readlineb(&rio, buf, BUF_SIZE);

    while (!(buf[0] == '\r' && buf[1] == '\n') && buf[0] != '\n')
    {
    	/* Lit chaque header */
    	/* Chaque header est composé d'un champ et d'un contenu séparé par ':'
		 * on utilise strtok car le contenu du champs peut contenir ':' ce qui trouble
		 * sscanf
		 */

    	field = strtok(buf, ":");
    	content = strtok(NULL, ": ");
		strlower(field);

		/* Dispatch chaque nom de champ vers le code de remplissage
		 * de la structure http_request_t. Les valeurs de hachage des
		 * noms des champs ont été précalculées et sont sans collision.
		 * On utilise un goto calculé, l'adresse du label est indiquée
		 * dans la table indexée par la valeur de hachage du nom.
		 * Ces gotos sont une extension gcc.
		 *
		 * http://eli.thegreenplace.net/2012/07/12/computed-goto-for-efficient-dispatch-tables/
		 */

		goto *request_header_table[hash(field)%TABLE_SIZE];

		accept :
			strncpy(request->accept, content, ACCEPT_MAX);
			goto fetch;

		accept_charset :
			strncpy(request->accept_charset, content,  ACCEPT_CHARSET_MAX);
			goto fetch;

		accept_encoding :
			strncpy(request->accept_encoding, content, ACCEPT_ENCODING_MAX);
			goto fetch;

		accept_language :
			strncpy(request->accept_language, content, ACCEPT_LANGUAGE_MAX);
			goto fetch;

		allow :
			if(!strcasecmp(content, "GET"))
			{
			  request->allow = ALLOW_GET;
			}
			else if(!strcasecmp(content, "HEAD"))
			{
			  request->allow = ALLOW_HEAD;
			}
			else if(!strcasecmp(content, "PUT"))
			{
			  request->allow = ALLOW_PUT;
			}
			goto fetch;

		authorization :
			strncpy(request->authorization,  content, AUTHORIZATION_MAX);
			goto fetch;

		content_encoding :
			strncpy(request->content_encoding, content, CONTENT_ENCODING_MAX);
			goto fetch;

		content_language :
			strncpy(request->content_language, content, CONTENT_LANGUAGE_MAX);
			goto fetch;

		content_length :
			request->content_length = atoi(content);
			goto fetch;

		content_location :
			strncpy(request->content_location, content, URI_MAX);
			goto fetch;

		content_md5 :
			strncpy(request->content_md5, content, CONTENT_MD5_MAX);
			goto fetch;

		content_range :
			strncpy(request->content_range, content, CONTENT_RANGE_MAX);
			goto fetch;

		content_type :
			strncpy(request->content_type, content, CONTENT_TYPE_MAX);
			goto fetch;

		expires :
			strptime(content, "%a, %d %b %Y %H:%M:%S GMT", &request->expires);
			goto fetch;

		expect :
			strncpy(request->expect, content, EXPECTATION_MAX);
			goto fetch;

		from :
			strncpy(request->from, content, FROM_MAX);
			goto fetch;

		host :
			strncpy(request->host, content, HOST_MAX);
			goto fetch;

		referer :
			strncpy(request->referer, content, URI_MAX);
			goto fetch;

		transfer_encoding :
			strncpy(request->transfer_encoding, content, TE_MAX);
			goto fetch;

		user_agent :
			strncpy(request->user_agent, content, USER_AGENT_MAX);
			goto fetch;

		cache_control :
			strncpy(request->cache_control, content, CACHE_CONTROL_MAX);
			goto fetch;

		connection :
			strncpy(request->connection,  content, CONNECTION_MAX);
			goto fetch;

		date :
			strptime(content, "%a, %d %b %Y %H:%M:%S GMT", &request->date);
			goto fetch;

		pragma :
			strncpy(request->pragma, content, PRAGMA_MAX);
			goto fetch;

		trailer :
			strncpy(request->trailer, content, TRAILER_MAX);
			goto fetch;

		upgrade :
			strncpy(request->upgrade, content, UPGRADE_MAX);
			goto fetch;

		via :
			strncpy(request->via, content, VIA_MAX);
			goto fetch;

		warning :
			sscanf(content, "%3i", &request->warn_code);
			goto fetch;

		last_modified :
			strptime(content, "%a, %d %b %Y %H:%M:%S GMT", &request->last_modified);
			goto fetch;

		not_supported :
			log_info("Request header field %s not supported", field);
			goto fetch;

		fetch :

        Rio_readlineb(&rio, buf, BUF_SIZE);
    }

    /* checks that a host header has been given
	 * http://www.w3.org/Protocols/rfc2616/rfc2616-sec14.html#sec14.23
	 */

    if(!*request->host)
	{
		http_clienterror(fd, HTTP_BAD_REQUEST, "No Host header");
		return -1;
	}

	return 0;
}


/*
typedef struct {

	int http_version_major;
	int http_version_minor;
	int status_code;
	char reason_phrase[REASON_PHRASE_MAX];


	char accept_ranges[ACCEPT_RANGES_MAX];
	int age;
	char etag[ETAG_MAX];
	char location[URI_MAX];
	char proxy_authenticate[PROXY_AUTH_MAX];
	char retry_after[RETRY_AFTER_MAX];
	char server[SERVER_MAX];
	char vary[VARY_MAX];
	char www_authenticate[WWW_AUTH_MAX];


	char cache_control[CACHE_CONTROL_MAX];
	char connection[CONNECTION_MAX];
	struct tm date;
	char pragma[PRAGMA_MAX];
	char trailer[TRAILER_MAX];
	char upgrade[UPGRADE_MAX];
	char via[VIA_MAX];
	int warn_code;

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
*/

/*
  See :
  http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html
 */
void http_response_write(int fd, http_response_t *response)
{
	char response_line[REPONSE_LINE_SIZE+1];
	char field_line[FIELD_MAX];
	assert(response);

	/* response line */
	sprintf(response_line, "HTTP/%1i.%1i %i %s\r\n", response->http_version_major,
			response->http_version_minor, response->status_code, response->reason_phrase);

	Rio_writen(fd, response_line, strlen(response_line));

	/*
	  Les lignes suivante vont regarder pour chaque champ du response header
	  si il est pertinent de l'envoyer ou pas, si oui,
	  on envoie le champ sur le descripteur de fichier de la socket
	 */
	if(*response->accept_ranges)
	{
		sprintf(field_line, "Accept-Ranges: %s\r\n", response->accept_ranges);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(response->age >= 0)
	{
		sprintf(field_line, "Age: %i\r\n", response->age);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->etag)
	{
		sprintf(field_line, "ETag: \"%s\"\r\n", response->etag);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->location)
	{
		sprintf(field_line, "Location: %s\r\n", response->location);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->proxy_authenticate)
	{
		sprintf(field_line, "Proxy-Authenticate: %s\r\n", response->proxy_authenticate);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->retry_after)
	{
		sprintf(field_line, "Retry-After: %s\r\n", response->retry_after);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	/* We add server name because we are proud of it */
	if(!*response->server)
		strcpy(response->server, SERVER_NAME);

	sprintf(field_line, "Server: %s\r\n", response->server);
	Rio_writen(fd, field_line, strlen(field_line));

	if(*response->vary)
	{
		sprintf(field_line, "Vary: %s\r\n", response->vary);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->www_authenticate)
	{
		sprintf(field_line, "WWW-Authenticate: %s\r\n", response->www_authenticate);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->cache_control)
	{
		sprintf(field_line, "Cache-Control: %s\r\n", response->cache_control);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->connection)
	{
		sprintf(field_line, "Connection: %s\r\n", response->connection);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(response->date.tm_year != 0) /* better solution ? */
	{
		sprintf(field_line, "Date: %s\r\n", http_date(&response->date));
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->pragma)
	{
		sprintf(field_line, "Pragma: %s\r\n", response->pragma);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->trailer)
	{
		sprintf(field_line, "Trailer: %s\r\n", response->trailer);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->upgrade)
	{
		sprintf(field_line, "Upgrade: %s\r\n", response->upgrade);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->via)
	{
		sprintf(field_line, "via: %s\r\n", response->via);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	/* We have to talk about the two nexts */
	if(response->warn_code)
		log_error("Reponse header field 'Warning' not implemented");

	if(response->allow)
		log_error("Reponse header field 'allow' not implemented");

	if(*response->content_encoding)
	{
		sprintf(field_line, "Content-Encoding: %s\r\n", response->content_encoding);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->content_language)
	{
		sprintf(field_line, "Content-Language: %s\r\n", response->content_language);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(response->content_length != -1)
	{
		sprintf(field_line, "Content-Length: %i\r\n", response->content_length);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->content_location)
	{
		sprintf(field_line, "Content-Location: %s\r\n", response->content_location);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->content_md5)
	{
		sprintf(field_line, "Content-MD5: %s\r\n", response->content_md5);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->content_range)
	{
		sprintf(field_line, "Content-Range: %s\r\n", response->content_range);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(*response->content_type)
	{
		sprintf(field_line, "Content-Type: %s\r\n", response->content_type);
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(response->expires.tm_year != 0)
	{
		sprintf(field_line, "Expires: %s\r\n", http_date(&response->expires));
		Rio_writen(fd, field_line, strlen(field_line));
	}

	if(response->last_modified.tm_year != 0)
	{
		sprintf(field_line, "Last-Modified: %s\r\n", http_date(&response->last_modified));
		Rio_writen(fd, field_line, strlen(field_line));
	}

	Rio_writen(fd, "\r\n", 2); /* On termine l'écriture du header */
}

void http_response_default(http_response_t *response, int http_major, int http_minor, int status)
{
	assert(response);
	bzero(response, sizeof(http_response_t));
	response->http_version_major = http_major;
	response->http_version_minor = http_minor;
	response->status_code = status;
	strcpy(response->reason_phrase, HTTP_STR(status));
	response->age = -1;
}
