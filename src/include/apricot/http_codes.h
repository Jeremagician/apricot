#ifndef _HTTP_CODES_
#define _HTTP_CODES_

/*
  Definition des codes de status http
  http://www.w3.org/Protocols/rfc2616/rfc2616-sec6.html#sec6.1.1
 */

/*
  1xx: Informational - Request received, continuing process
 */
#define HTTP_CONTINUE 100
#define HTTP_SWITCHING_PROTOCOLS 101

/*
  2xx: Success - The action was successfully received,
  understood, and accepted
 */
#define HTTP_OK 200
#define HTTP_CREATED 201
#define HTTP_ACCEPTED 202
#define HTTP_NO_CONTENT 204
#define HTTP_PARTIAL 206
#define HTTP_RESET_CONTENT 205
#define HTTP_PARTIAL_CONTENT 206

/*
  3xx: Redirection - Further action must be taken in order to
  complete the request
 */
#define HTTP_MULTIPLE_CHOICES 300
#define HTTP_MOVED 301
#define HTTP_FOUND 302
#define HTTP_SEE_OTHER 303
#define HTTP_NOT_MODIFIED 304
#define HTTP_USE_PROXY 305
#define HTTP_TEMPORARY_REDIRECT	307

/*
  4xx: Client Error - The request contains bad syntax or cannot
  be fulfilled
 */
#define HTTP_BAD_REQUEST 400
#define HTTP_UNAUTHORIZED 401
#define HTTP_PAYMENT_REQUIRED 402
#define HTTP_FORBIDDEN 403
#define HTTP_NOT_FOUND 404
#define HTTP_METHOD_NOT_ALLOWED 405
#define HTTP_NOT_ACCEPTABLE 406
#define HTTP_REQUEST_TIMEOUT 408
#define HTTP_URI_TOO_LONG 414
#define HTTP_UNSUPPORTED_MEDIA_TYPE	415
#define HTTP_RANGE_ERROR 416

/*
  Server Error - The server failed to fulfill an apparently
  valid request
 */
#define HTTP_INTERNAL_ERROR	500
#define HTTP_NOT_IMPLEMENTED 501
#define HTTP_BAD_GATEWAY 502
#define HTTP_UNAVAILABLE 503
#define HTTP_GATEWAY_TIMEOUT 504
#define HTTP_VERSION_NOT_SUPPORTED 505

/*
  Macro permettant d'obtenir la "Reason Phrase" selon le status code HTTP
 */
#define HTTP_STR(x) (\
    (x==HTTP_CONTINUE)				 ? "Continue" :\
	(x==HTTP_SWITCHING_PROTOCOLS)    ? "Switching Protocols" :\
	(x==HTTP_OK)				     ? "OK" :\
    (x==HTTP_CREATED)				 ? "Created" :\
	(x==HTTP_ACCEPTED)				 ? "Accepted" :\
	(x==HTTP_NO_CONTENT)		     ? "No Content" :\
	(x==HTTP_PARTIAL)			     ? "Partial Content" :\
	(x==HTTP_MULTIPLE_CHOICES)	     ? "Multiple Choices" :\
	(x==HTTP_MOVED)				     ? "Moved Permanently" :\
	(x==HTTP_FOUND)				     ? "Found" :\
	(x==HTTP_METHOD_NOT_ALLOWED)     ? "Method Not Allowed" : \
	(x==HTTP_SEE_OTHER)			     ? "See Other" :\
	(x==HTTP_NOT_MODIFIED)		     ? "Not Modified" :\
	(x==HTTP_TEMPORARY_REDIRECT)     ? "Temporary Redirect" :\
	(x==HTTP_BAD_REQUEST)		     ? "Bad Request" :\
	(x==HTTP_UNAUTHORIZED)		     ? "Unauthorized" :\
	(x==HTTP_PAYMENT_REQUIRED)	     ? "Payment Required" :\
	(x==HTTP_FORBIDDEN)			     ? "Forbidden" :\
	(x==HTTP_NOT_FOUND)			     ? "Not Found" :\
	(x==HTTP_NOT_ACCEPTABLE)	     ? "Not Acceptable" :\
	(x==HTTP_REQUEST_TIMEOUT)	     ? "Request Time-out" :\
	(x==HTTP_URI_TOO_LONG)		     ? "Request-URI Too Large" :\
	(x==HTTP_UNSUPPORTED_MEDIA_TYPE) ? "Unsupported Media Type" :\
	(x==HTTP_RANGE_ERROR)		     ? "Requested range not satisfiable" :\
	(x==HTTP_INTERNAL_ERROR)	     ? "Internal Server Error" :\
	(x==HTTP_NOT_IMPLEMENTED)	     ? "Not Implemented" :\
	(x==HTTP_BAD_GATEWAY)		     ? "Bad Gateway" :\
	(x==HTTP_UNAVAILABLE)		     ? "Service Unavailable" :\
	(x==HTTP_GATEWAY_TIMEOUT)	     ? "Gateway Time-out" :\
	(x==HTTP_VERSION_NOT_SUPPORTED)  ? "HTTP Version not supported":\
   	"invalid" )

/*
  Macro pour verifier le type du code de retour
 */
#define HTTP_IS_INFORMATIONAL(n) (n=>100 && n<200)
#define HTTP_IS_SUCCESS(n)		 (n>=200 && n<300)
#define HTTP_IS_REDIRECT(n)		 (n>=300 && n<400)
#define HTTP_IS_CLIENT_ERROR(n)  (n>=400 && n<500)
#define HTTP_IS_SERVER_ERROR(n)  (n>=500 && n<600)

#endif
