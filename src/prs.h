/* {{{ */
/*
	Copyright (c) 2018, Mihail Szabolcs

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the 'Software'), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in all
	copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
	SOFTWARE.
*/
/* }}} */
#ifndef PRS_H
#define PRS_H

#ifndef PRS_NO_STDIO
	#include <stdio.h>
#endif

#ifndef PRS_NO_CTYPE
	#include <ctype.h>
#endif

#ifndef PRS_VERSION
	#define PRS_VERSION 0x101
#endif

#ifndef PRS_UINT_TYPE
	#define PRS_UINT_TYPE unsigned int
#endif

#ifndef PRS_CHAR_TYPE
	#define PRS_CHAR_TYPE char
#endif

#ifndef PRS_UNUSED
	#define PRS_UNUSED(x) (void)(x)
#endif

#ifndef PRS_ARRAY_SIZE
	#define PRS_ARRAY_SIZE(a) (sizeof((a)) / sizeof((a)[0]))
#endif

#ifndef PRS_EOS
	#define PRS_EOS '\0'
#endif

#ifndef PRS_EOL
	#define PRS_EOL '\n'
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifndef PRS_API
	#ifdef PRS_STATIC
		#define PRS_API static
	#else
		#define PRS_API
	#endif
#endif

typedef PRS_UINT_TYPE prs_uint_t;
typedef PRS_CHAR_TYPE prs_char_t;

typedef enum
{
	PRS_TOKEN_TYPE_INVALID		= 0,
	PRS_TOKEN_TYPE_SYMBOL		= 1,
	PRS_TOKEN_TYPE_IDENTIFIER	= 2,
	PRS_TOKEN_TYPE_NUMBER		= 3,
	PRS_TOKEN_TYPE_STRING		= 4
} prs_token_type_t;

typedef struct
{
	prs_token_type_t type;
	prs_uint_t len;
	prs_uint_t line;
	const prs_char_t *s;
	prs_char_t c;
} prs_token_t;

typedef struct
{
	prs_char_t *s;
	prs_uint_t line;
} prs_context_t;

PRS_API void prs_init(prs_context_t *ctx, const prs_char_t *s);
PRS_API int prs_parse(prs_context_t *ctx, prs_token_t *token);
PRS_API int prs_parse_expect(prs_context_t *ctx, prs_token_t *token, const prs_char_t *s);
PRS_API int prs_unparse(prs_context_t *ctx, prs_token_t *token);
PRS_API int prs_token_compare(const prs_token_t *token, const prs_char_t *s);
PRS_API prs_uint_t prs_token_copy(const prs_token_t *token, prs_char_t *s, const prs_uint_t num);
PRS_API const char *prs_token_type_to_str(const prs_token_t *token);

#ifdef __cplusplus
}
#endif

#endif

#ifdef PRS_IMPLEMENTATION

#ifdef __GNUC__
	#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Wunused-function"
#endif

#ifndef prs_isspace
	#define prs_isspace(c) isspace(c)
#endif

#ifndef prs_isnewline
	#define prs_isnewline(c) (c == PRS_EOL)
#endif

#ifndef prs_iseos
	#define prs_iseos(c) (c == PRS_EOS)
#endif

#ifndef prs_issingleline_comment
	#define prs_issingleline_comment(c0, c1) (c0 == '/' && c1 == '/')
#endif

#ifndef prs_ismultiline_comment_start
	#define prs_ismultiline_comment_start(c0, c1) (c0 == '/' && c1 == '*')
#endif

#ifndef prs_ismultiline_comment_end
	#define prs_ismultiline_comment_end(c0, c1) (c0 == '*' && c1 == '/')
#endif

#ifndef prs_isidentifier
	#define prs_isidentifier(c) (isalnum(c) || c == '_')
#endif

#ifndef prs_isalpha
	#define prs_isalpha(c) isalpha(c)
#endif

#ifndef prs_isdigit
	#define prs_isdigit(c) isdigit(c)
#endif

#ifndef prs_isxdigit
	#define prs_isxdigit(c) isxdigit(c)
#endif

#ifndef prs_isdigithex
	#define prs_isdigithex(c0, c1) (c0 == '0' && (c1 == 'x' || c1 == 'X'))
#endif

#ifndef prs_isdigitsign
	#define prs_isdigitsign(c) (c == '-' || c == '+')
#endif

#ifndef prs_isdigitsep
	#define prs_isdigitsep(c) (c == '.' || c == 'e')
#endif

#ifndef prs_isdigittype
	#define prs_isdigittype(c) (c == 'L' || c == 'U')
#endif

#ifndef prs_isstring
	#define prs_isstring(c) (c == '"' || c == '\'')
#endif

#ifndef prs_isescape
	#define prs_isescape(c) (c == '\\')
#endif

#ifndef prs_ispreprocessor
	#define prs_ispreprocessor(c) (c == '#')
#endif

#ifdef PRS_PARSE_MULTILINE_STRING
	#ifndef prs_ismultiline_string
		#define prs_ismultiline_string(c0, c1, c2) (c0 == '"' && c1 == '"' && c2 == '"')
	#endif
#endif

PRS_API void prs_init(prs_context_t *ctx, const prs_char_t *s)
{
	ctx->s = (prs_char_t *) s;
	ctx->line = 1;
}

PRS_API int prs_parse(prs_context_t *ctx, prs_token_t *token)
{
	prs_char_t *s;
	prs_uint_t len, line;

	s = ctx->s;
	len = 0;
	line = ctx->line;

	token->s = NULL;
	token->c = 0;
	token->type = PRS_TOKEN_TYPE_INVALID;
	token->len = 0;
	token->line = 0;

	if(prs_iseos(*s))
		return 0;

	for(;;)
	{
		while(!prs_iseos(*s))
		{
			if(!prs_isspace(*s))
				break;

			if(prs_isnewline(*s))
				line++;

			s++;
		}

#ifndef PRS_PARSE_PREPROCESSOR
		if(prs_ispreprocessor(*s))
		{
			while(!prs_iseos(*s) && !prs_isnewline(*s))
			{
				if(prs_isescape(*s) && prs_isnewline(*(s + 1)))
				{
					line++;
					s++;
				}

				s++;
			}

			continue;
		}
#endif

		if(prs_issingleline_comment(*s, *(s + 1)))
		{
			while(!prs_iseos(*s) && !prs_isnewline(*s))
				s++;

			continue;
		}
		else if(prs_ismultiline_comment_start(*s, *(s + 1)))
		{
			while(!prs_iseos(*s) && !prs_ismultiline_comment_end(*s, *(s + 1)))
			{
				if(prs_isnewline(*s++))
					line++;
			}

			if(!prs_iseos(*s) && prs_ismultiline_comment_end(*s, *(s + 1)))
			{
				s++;
				s++;
			}

			continue;
		}

		break;
	}

	if(prs_iseos(*s))
	{
		ctx->s = s;
		ctx->line = line;
		return 0;
	}

	token->line = line;
	token->s = s;
	token->c = *s;

	if(prs_isdigit(*s) || (prs_isdigitsign(*s) && prs_isdigit(*(s + 1))))
	{
		if(prs_isdigitsign(*s))
		{
			len++;
			s++;
		}

		if(prs_isdigithex(*s, *(s + 1)))
		{
			len++;
			s++;

			len++;
			s++;

			while(prs_isxdigit(*s))
			{
				len++;
				s++;
			}
		}
		else
		{
			while(prs_isdigit(*s) || prs_isdigitsep(*s))
			{
				len++;
				s++;
			}
		}

		while(prs_isdigittype(*s))
		{
			len++;
			s++;
		}

		token->type = PRS_TOKEN_TYPE_NUMBER;
		token->len = len;

		if(prs_isalpha(*s))
			return 0;
	}
	else if(prs_isidentifier(*s))
	{
		while(prs_isidentifier(*s))
		{
			len++;
			s++;
		}

		token->type = PRS_TOKEN_TYPE_IDENTIFIER;
		token->len = len;
	}
#ifdef PRS_PARSE_MULTILINE_STRING
	else if(prs_ismultiline_string(*s, *(s + 1), *(s + 2)))
	{
		s++;
		s++;
		s++;

		while(!prs_iseos(*s) && !prs_ismultiline_string(*s, *(s + 1), *(s + 2)))
		{
			if(prs_isnewline(*s))
			{
				line++;
			}
			else if(prs_isescape(*s) && prs_isstring(*(s + 1)))
			{
				len++;
				s++;
			}

			len++;
			s++;
		}

		token->type = PRS_TOKEN_TYPE_STRING;
		token->len = len;
		token->s = s - len;

		if(!prs_ismultiline_string(*s, *(s + 1), *(s + 2)))
			return 0;

		s++;
		s++;
		s++;
	}
#endif
	else if(prs_isstring(*s))
	{
		prs_char_t c = *s;

		s++;

		while(!prs_iseos(*s) && *s != c)
		{
			if(prs_isnewline(*s))
			{
				line++;
			}
			else if(prs_isescape(*s) && *(s + 1) == c)
			{
				len++;
				s++;
			}

			len++;
			s++;
		}

		token->type = PRS_TOKEN_TYPE_STRING;
		token->len = len;
		token->s = s - len;

		if(!prs_isstring(*s))
			return 0;

		s++;
	}
	else
	{
		s++;

		token->type = PRS_TOKEN_TYPE_SYMBOL;
		token->len = 1;
	}

	ctx->s = s;
	ctx->line = line;
	return 1;
}

PRS_API int prs_parse_expect(prs_context_t *ctx, prs_token_t *token, const prs_char_t *s)
{
	int ret;

	ret = prs_parse(ctx, token);
	if(ret)
		return prs_token_compare(token, s);

	return ret;
}

PRS_API int prs_unparse(prs_context_t *ctx, prs_token_t *token)
{
	if(token->type == PRS_TOKEN_TYPE_INVALID || token->s == NULL || token->line < 1)
		return 0;

	ctx->s = (prs_char_t *) ((token->type == PRS_TOKEN_TYPE_STRING) ? token->s - 1 : token->s);
	ctx->line = token->line;

	return 1;
}

PRS_API int prs_token_compare(const prs_token_t *token, const prs_char_t *s)
{
	prs_uint_t i, len;
	const prs_char_t *src;

	i = 0;
	src = token->s;
	len = token->len;

	if((src == NULL || prs_iseos(*src)) && (s == NULL || prs_iseos(*s)))
		return 1;

	if(src == NULL || s == NULL)
		return 0;

	while(!prs_iseos(*src) && !prs_iseos(*s))
	{
		if(prs_isescape(*src) && prs_isstring(*(src + 1)))
		{
			src++;
			len--;
		}

		if(*src++ != *s++)
			return 0;

		if(i++ == len)
			break;
	}

	return i == len && prs_iseos(*s);
}

PRS_API prs_uint_t prs_token_copy(const prs_token_t *token, prs_char_t *s, const prs_uint_t num)
{
	prs_uint_t i, j, len, max;
	const prs_char_t *src;

	max = num - 1;
	src = token->s;
	len = token->len;

	for(i = 0, j = 0; i < max && j < len; i++, j++)
	{
		if(prs_isescape(src[j]) && prs_isstring(src[j + 1]))
			j++;

		s[i] = src[j];
	}

	s[i] = PRS_EOS;
	return i;
}

PRS_API const char *prs_token_type_to_str(const prs_token_t *token)
{
	switch(token->type)
	{
		case PRS_TOKEN_TYPE_INVALID:
			return "invalid";

		case PRS_TOKEN_TYPE_SYMBOL:
			return "symbol";

		case PRS_TOKEN_TYPE_IDENTIFIER:
			return "identifier";

		case PRS_TOKEN_TYPE_NUMBER:
			return "number";

		case PRS_TOKEN_TYPE_STRING:
			return "string";
	}

	return "unknown";
}

#ifdef __GNUC__
	#pragma GCC diagnostic pop
#endif

#endif

/* vim: set ts=4 sw=4 sts=4 noet: */
