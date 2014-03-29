#include <stdio.h>
#include <stdint.h>

/* COMPUTE HASH VALUES OF REQUEST HEADERS FIELDS */

uint32_t hash (char* str)
{
	uint32_t h = 0;
	
	while(*str)
	{
		h = (h<<5) + h + *str;
		str++;
	}
	
	return h;
}

int main()
{
	printf("#define HASH_ACCEPT %i\n", hash("accept")%1000);
	printf("#define HASH_ACCEPT_CHARSET %i\n", hash("accept-charset")%1000);
	printf("#define HASH_ACCEPT_ENCODING %i\n", hash("accept-encoding")%1000);
	printf("#define HASH_ACCEPT_LANGUAGE %i\n", hash("accept-language")%1000);
	printf("#define HASH_AUTHORIZATION %i\n", hash("authorization")%1000);
	printf("#define HASH_EXCEPT %i\n", hash("except")%1000);
	printf("#define HASH_FROM %i\n", hash("from")%1000);
	printf("#define HASH_HOST %i\n", hash("host")%1000);
	printf("#define HASH_IF_MATCH %i\n", hash("if-match")%1000);
	printf("#define HASH_IF_MODIFIED_SINCE %i\n", hash("if-modified-since")%1000);
	printf("#define HASH_IF_NONE_MATCH %i\n", hash("if-none-match")%1000);
	printf("#define HASH_RANGE %i\n", hash("if-range")%1000);
	printf("#define HASH_UNMODIFIED_SINCE %i\n", hash("if-unmodified-since")%1000);
	printf("#define HASH_MAX_FORWARDS %i\n", hash("max-forwards")%1000);
	printf("#define HASH_PROXY_AUTHORIZATION %i\n", hash("proxy_authorization")%1000);
	printf("#define HASH_RANGE %i\n", hash("range")%1000);
	printf("#define HASH_REFERER %i\n", hash("referer")%1000);
	printf("#define HASH_TE %i\n", hash("te")%1000);
	printf("#define HASH_USER_AGENT %i\n", hash("user-agent")%1000);
	printf("#define HASH_CACHE_CONTROL %i\n", hash("cache-control")%1000);
	printf("#define HASH_CONNECTION %i\n", hash("connection")%1000);
	printf("#define HASH_DATE %i\n", hash("date")%1000);
	printf("#define HASH_PRAGMA %i\n", hash("pragma")%1000);
	printf("#define HASH_TRAILER %i\n", hash("trailer")%1000);
	printf("#define HASH_TRANSFER_ENCODING %i\n", hash("transfer-encoding")%1000);
	printf("#define HASH_UPGRADE %i\n", hash("upgrade")%1000);
	printf("#define HASH_VIA %i\n", hash("via")%1000);
	printf("#define HASH_WARNING %i\n", hash("warning")%1000);

	return 0;
}
