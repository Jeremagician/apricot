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
static int has_post = 0;
static int has_get = 0;

static void cgi_init_get();
static void cgi_init_post();
static int parse_data(char * buf, prefix_tree_t *t);

void cgi_init()
{
	POST = prefix_tree_new();
	GET = prefix_tree_new();

	if(POST) cgi_init_post();
	if(GET) cgi_init_get();
}

void cgi_exit()
{
	prefix_tree_free(POST);
	prefix_tree_free(GET);
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

int cgi_has_get()
{
	return has_get;
}

int cgi_has_post()
{
	return has_post;
}
