#include <apricot/http_header.h>
#include <apricot/csapp.h>
#include <apricot/http_codes.h>
#include <apricot/http_error.h>
#include <apricot/log.h>
#include <apricot/utils.h>
#include <string.h>
#include <stdint.h>

#define BUF_SIZE METHOD_MAX + URI_MAX + 10
#define TABLE_SIZE 1000

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
	
	log_info("GET %s HTTP/%i.%i", request->uri, request->http_version_major, request->http_version_minor);
	
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
		
		authorization :
			strncpy(request->authorization,  content, AUTHORIZATION_MAX);
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
			strncpy(request->date, content, DATE_MAX);
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
			
		not_supported :
			log_info("Request header field %s not supported", field);
			goto fetch;
		
		fetch :
    	
        Rio_readlineb(&rio, buf, BUF_SIZE);
    }
	
	return 0;
}
