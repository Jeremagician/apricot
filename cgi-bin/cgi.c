#include "cgi.h"
#include <stdlib.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define CHAR_MIN 32
#define CHAR_MAX 126
#define CHAR_RANGE CHAR_MAX-CHAR_MIN

typedef struct prefix_tree_t
{
	struct prefix_tree_t* table[CHAR_RANGE];
    char * data;
} prefix_tree_t;

static prefix_tree_t* prefix_tree_new();
static void prefix_tree_set(prefix_tree_t *tree, char* key, char* data);
static void* prefix_tree_get(prefix_tree_t *tree, char* key);
static void prefix_tree_free(prefix_tree_t *tree);

static prefix_tree_t *POST;
static prefix_tree_t *GET;
static prefix_tree_t *COOKIE;
static int has_post = 0;
static int has_get = 0;
static int has_cookie = 0;

static void cgi_init_get();
static void cgi_init_post();
static void cgi_init_cookie();
static int parse_data(char * buf, prefix_tree_t *t);
static void cgi_write_cookies();

void cgi_init()
{
	POST = prefix_tree_new();
	GET = prefix_tree_new();
	POST = prefix_tree_new();
	COOKIE = prefix_tree_new();

	if(POST) cgi_init_post();
	if(GET) cgi_init_get();
	if(COOKIE) cgi_init_cookie();
}

void cgi_exit()
{
	prefix_tree_free(POST);
	prefix_tree_free(GET);
	cgi_write_cookies();
	prefix_tree_free(COOKIE);
}

int cgi_post_isset(char * key)
{
	return prefix_tree_get(POST, key) != NULL;
}

char * cgi_post_value(char * key)
{
	return prefix_tree_get(POST, key);
}

int cgi_get_isset(char * key)
{
	return prefix_tree_get(GET, key) != NULL;
}

char * cgi_get_value(char * key)
{
	return prefix_tree_get(GET, key);
}

int cgi_cookie_isset(char * key)
{
	return prefix_tree_get(COOKIE, key) != NULL;
}

char * cgi_cookie_get(char * key)
{
	return prefix_tree_get(COOKIE, key);
}

void cgi_cookie_set(char * key, char * value)
{
	return prefix_tree_set(COOKIE, key, value);
}

static prefix_tree_t* prefix_tree_new()
{
	prefix_tree_t *tree = malloc(sizeof(prefix_tree_t));
    if(tree)
	{
		int i;
		for(i=0;i<CHAR_RANGE;i++)
			tree->table[i] = NULL;

		tree->data = NULL;
	}
	return tree;
}

static void prefix_tree_set(prefix_tree_t *tree, char* key, char* data)
{
	size_t i = 0;
	prefix_tree_t * cur = tree;

	if(!key[i]) return;

	while(key[i])
	{
		if(cur->table[key[i] - CHAR_MIN] == NULL)
			cur->table[key[i] - CHAR_MIN] = prefix_tree_new();

		cur = cur->table[key[i] - CHAR_MIN];
		i++;
	}
	cur->data = strdup(data);
}

static void* prefix_tree_get(prefix_tree_t *tree, char* key)
{
	size_t i = 0;
	prefix_tree_t * cur = tree;

	while(key[i] && cur)
	{
		cur = cur->table[key[i] - CHAR_MIN];
		i++;
	}

	if(cur)
		return cur->data;
	else
		return NULL;
}

static void prefix_tree_free(prefix_tree_t *tree)
{
	if(!tree)
		return;
	else
	{
		int i;
		for(i=0;i<CHAR_RANGE;i++)
			prefix_tree_free(tree->table[i]);

		free(tree->data);
		free(tree);
	}
}

/*
  returns how many data parsed
 */
static int parse_data(char * buf, prefix_tree_t *t)
{
	char *p;
	char *end;
	int i = 0;

	p = strtok_r(buf, "&", &end);
	while(p != NULL)
	{
		char *val;
		char *key = strtok_r(p, "=", &val);

		if(*key && *val)
		{
			prefix_tree_set(t, key, val);
			i++;
		}

		p = strtok_r(NULL, "&", &end);
	}
	return i;
}

static void cgi_init_get()
{
	char* buf;
    if ((buf = getenv("QUERY_STRING")) != NULL && *buf) {
		has_get = parse_data(buf, GET);
	}
}

static void cgi_init_post()
{
	char * buf;
	if((buf = getenv("CONTENT_LENGTH")) != NULL)
	{
		int content_length = atoi(buf);
		char post_buf[content_length];
		read(STDIN_FILENO, post_buf, content_length);
		has_post = parse_data(post_buf, POST);
	}
}

static char from_hex(char ch) {
  return isdigit(ch) ? ch - '0' : tolower(ch) - 'a' + 10;
}

static char to_hex(char code) {
  static char hex[] = "0123456789abcdef";
  
  return hex[code & 15];
}

/* encode une chaine valeur pour un fichier de cookies */
/* doit etre absolument liberee */
static char * url_encode(char *str) {
  char *buf = malloc(strlen(str) * 3 + 1);
  char *pbuf = buf;
  
  while (*str) 
  {
    if (isalnum(*str) || *str == '-' || *str == '_' || *str == '.' || *str == '~') 
    {
      *pbuf++ = *str;
    }
    else if (*str == ' ')
    { 
      *pbuf++ = '+';
    }
    else
    { 
      *pbuf++ = '%';
      *pbuf++ = to_hex(*str >> 4);
      *pbuf++ = to_hex(*str & 15);
    }
    
    str++;
  }
  
  *pbuf = '\0';
  
  return buf;
}

/* decode une chaine valeur dans un fichier de cookies */
/* la chaine renvoyee doit etre liberee */
static char * url_decode(char *str) {
  char *buf = malloc(strlen(str) + 1);
  char *pbuf = buf;
  
  while (*str) 
  {
  	if (*str == '%') 
    {
      if (*(str+1) && *(str+2)) 
      {
        *pbuf++ = from_hex(*(str + 1)) << 4 | from_hex(*(str + 2));
        str += 2;
      }
    } 
    else if (*str == '+') 
    { 
      *pbuf++ = ' ';
    } 
    else 
    {
      *pbuf++ = *str;
    }
    
    str++;
  }
  
  *pbuf = '\0';
  
  return buf;
}

static void cgi_init_cookie()
{
	char * buf;
	char key[COOKIE_KEY_MAX];
	char value[COOKIE_VALUE_MAX];
	
	if((buf = getenv("COOKIE")) != NULL && *buf)
	{
		FILE * f = fopen(buf, "r");
		
		if(!f)
		{
			exit(EXIT_FAILURE);
		}
		
		has_cookie = 1;
		
		while(!feof(f))
		{
			fscanf(f, "%511s %8191s", key, value);
			
			char * decoded = url_decode(value);
			strcpy(value, decoded);
			free(decoded);
				
			prefix_tree_set(COOKIE, key, value);
		}
		
		fclose(f);
	}
}

static void cgi_write_cookies_recursive(FILE * f, prefix_tree_t * tree)
{
	if(tree)
	{
		if(tree->data)
		{
			char * encode = url_encode(tree->data);
			fprintf(f, " %s\n", encode);
			free(encode);
		}
		else
		{
			int i;
			
			for(i = 0; i < CHAR_RANGE; i++)
			{
				if(tree->table[i])
				{
					fputc((char)CHAR_MIN+i, f);
					cgi_write_cookies_recursive(f, tree->table[i]);
				}
			}
		}
	}
}

static void cgi_write_cookies()
{
	char * buf;
	char tmp[COOKIE_KEY_MAX + COOKIE_VALUE_MAX];
	char key[COOKIE_KEY_MAX];
	char value[COOKIE_VALUE_MAX];
	
	if((buf = getenv("COOKIE")) != NULL && *buf)
	{
		truncate(buf, 0);
		FILE * f = fopen(buf, "w");
		
		if(!f)
		{
			exit(EXIT_FAILURE);
		}
		
		cgi_write_cookies_recursive(f, COOKIE);
		
		fclose(f);
	}
}

int cgi_has_cookie()
{
	return has_cookie;
}

int cgi_has_get()
{
	return has_get;
}

int cgi_has_post()
{
	return has_post;
}
