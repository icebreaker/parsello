#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRS_IMPLEMENTATION
#include <prs.h>

#ifndef MAX_KEYVALS
	#define MAX_KEYVALS 16
#endif

#ifndef MAX_STR
	#define MAX_STR 16
#endif

typedef enum
{
	KEYVALUE_TYPE_INT,
	KEYVALUE_TYPE_STR
} keyvalue_type_t;

typedef struct
{
	char name[MAX_STR];
	char svalue[MAX_STR];
	int ivalue;
	int type;
} keyvalue_t;

void parse(const char *s);
void print_parse_expect_error(const prs_token_t *token, const char *s);
char *read_file_contents(const char *filename);

int main(int argc, char *argv[])
{
	char *buffer;

	if(argc < 2)
	{
		printf("usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}

	buffer = read_file_contents(argv[1]);
	if(buffer == NULL)
	{
		fprintf(stderr, "Failed to read file: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	parse(buffer);

	free(buffer);
	return EXIT_SUCCESS;
}

void parse(const char *s)
{
	keyvalue_t keyvals[MAX_KEYVALS];
	int num_keyvals, i;
	prs_context_t ctx;
	prs_token_t token;

	prs_init(&ctx, s);

	if(!prs_parse_expect(&ctx, &token, "config"))
	{
		print_parse_expect_error(&token, "config");
		return;
	}

	if(!prs_parse_expect(&ctx, &token, "{"))
	{
		print_parse_expect_error(&token, "{");
		return;
	}

	num_keyvals = 0;

	while(num_keyvals < MAX_KEYVALS)
	{
		keyvalue_t *kv = &keyvals[num_keyvals];

		if(!prs_parse(&ctx, &token))
			break;

		if(prs_token_compare(&token, "}"))
			break;

		prs_token_copy(&token, kv->name, MAX_STR);

		if(!prs_parse_expect(&ctx, &token, "="))
			break;

		if(!prs_parse(&ctx, &token))
			break;

		if(token.type == PRS_TOKEN_TYPE_STRING)
		{
			prs_token_copy(&token, kv->svalue, MAX_STR);
		}
		else if(token.type == PRS_TOKEN_TYPE_NUMBER)
		{
			prs_token_copy(&token, kv->svalue, MAX_STR);
			kv->ivalue = atoi(kv->svalue);
		}
		else
		{
			continue;
		}

		num_keyvals++;
	}

	if(!prs_token_compare(&token, "}"))
	{
		print_parse_expect_error(&token, "}");
		return;
	}

	for(i = 0; i < num_keyvals; i++)
	{
		keyvalue_t *kv = &keyvals[i];
		printf("%s = %s\n", kv->name, kv->svalue);
	}
}

void print_parse_expect_error(const prs_token_t *token, const char *s)
{
	if(token->type == PRS_TOKEN_TYPE_INVALID)
		fprintf(stderr, "Parse Error: expected '%s' but reached end of string\n", s);
	else
		fprintf(stderr, "Parse Error: expected '%s' but got '%.*s' on line %u\n",
				s, token->len, token->s, token->line);
}

char *read_file_contents(const char *filename)
{
	char *buffer;
	size_t size;
	FILE *fp;

	fp = fopen(filename, "r");
	if(fp == NULL)
		return NULL;

	fseek(fp, 0L, SEEK_END);
	size = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	buffer = (char *) malloc(size + 1);
	if(buffer == NULL)
	{
		fclose(fp);
		return NULL;
	}

	if(fread(buffer, sizeof(char), size, fp) != size)
	{
		free(buffer);
		buffer = NULL;
	}
	else
	{
		buffer[size] = '\0';
	}

	fclose(fp);
	return buffer;
}

