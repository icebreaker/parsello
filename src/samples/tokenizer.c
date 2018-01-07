#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PRS_IMPLEMENTATION
#include <prs.h>

char *read_file_contents(const char *filename);

int main(int argc, char *argv[])
{
	prs_context_t ctx;
	prs_token_t token;
	char *buffer;
	int nostdout;

	if(argc < 2)
	{
		printf("usage: %s file\n", argv[0]);
		return EXIT_FAILURE;
	}

	if(argc > 2 && !strcmp(argv[2], "--no-stdout"))
		nostdout = 1;
	else
		nostdout = 0;

	buffer = read_file_contents(argv[1]);
	if(buffer == NULL)
	{
		fprintf(stderr, "Failed to read file: %s\n", argv[1]);
		return EXIT_FAILURE;
	}

	prs_init(&ctx, buffer);

	if(nostdout)
	{
		while(prs_parse(&ctx, &token))
			continue;

		printf("%d lines ...\n", ctx.line);
	}
	else
	{
		while(prs_parse(&ctx, &token))
			printf("%.*s\n", token.len, token.s);
	}

	free(buffer);
	return EXIT_SUCCESS;
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

