#include <string.h>

#define TESTA_REPORT_ASSERTS
#define TESTA_VIM_ASSERT
#include <tests/testa.h>

#define PRS_IMPLEMENTATION
#define PRS_PARSE_MULTILINE_STRING
#include <prs.h>

static int init(void)
{
	prs_context_t ctx;
	const char *s = "int main(void) { }";

	prs_init(&ctx, s);

	TESTA_ASSERT(ctx.s == s);
	TESTA_ASSERT(ctx.line = 1);

	ctx.s = NULL;
	ctx.line = 13;

	prs_init(&ctx, s);

	TESTA_ASSERT(ctx.s == s);
	TESTA_ASSERT(ctx.line = 1);

	return 0;
}

static int parse(void)
{
	prs_context_t ctx;
	prs_token_t token;
	const char *s = "char *main(void) { return \"\"; }";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == 'c');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '*');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == 'm');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '(');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == 'v');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == ')');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '{');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 6);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == 'r');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '"');
	TESTA_ASSERT(prs_token_compare(&token, ""));

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == ';');

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '}');

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(token.s == NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);
	TESTA_ASSERT(token.c == 0);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 1);

	return 0;
}

static int parse_expect(void)
{
	prs_context_t ctx;
	prs_token_t token;
	const char *s = "#define _WIN32 666 \\\n\t+ 333\n\n\t/* hello\n world */ \t\t\t\t\nint     main( void){\n"
		"// heheheheh\n\n\tint xyz =13; /* mememee\n */float /* yes */y=  \t42.13  \t;\n"
		"const\t char\t *s\n=\"hello \\\"world\\\" parsers\" \t;\n"
		"const    char c = '6'\t\t\t;\nreturn -33  \t   \n;\n}\n"
		"/* once again */\t\n// hehe\n\n\t"
		"float x =    13.4e0;\n"
		" float    y    = -13.32;\n"
		"int z =   \t\t\t 0x13666;\n"
		" int w \t\t\t\t\t= -0x14;\n"
		" int i    = 666L;\n"
		"int j    = 42UL       ;\n"
		"  int k=-4U      ;\n /* the end */";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "main"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "("));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "void"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ")"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "{"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 6);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "xyz"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "13"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 2);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "float"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "y"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "42.13"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "const"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 11);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "char"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 11);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "*"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 11);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "s"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 11);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 12);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "hello \"world\" parsers"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 23);
	TESTA_ASSERT(token.line == 12);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 12);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "const"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "char"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "c"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "6"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 13);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "return"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 6);
	TESTA_ASSERT(token.line == 14);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "-33"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 14);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 15);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "}"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 16);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "float"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 20);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "x"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 20);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 20);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "13.4e0"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 6);
	TESTA_ASSERT(token.line == 20);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 20);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "float"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 21);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "y"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 21);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 21);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "-13.32"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 6);
	TESTA_ASSERT(token.line == 21);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 21);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 22);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "z"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 22);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 22);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "0x13666"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 7);
	TESTA_ASSERT(token.line == 22);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 22);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 23);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "w"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 23);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 23);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "-0x14"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 23);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 23);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 24);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "i"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 24);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 24);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "666L"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 24);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 24);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 25);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "j"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 25);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 25);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "42UL"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 25);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 25);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "int"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 26);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "k"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 26);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 26);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "-4U"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 26);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 26);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(prs_token_compare(&token, ""));
	TESTA_ASSERT(prs_token_compare(&token, NULL));
	TESTA_ASSERT(token.s == NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 27);

	return 0;
}

static int unparse(void)
{
	prs_context_t ctx;
	prs_token_t token, token2;
	const char *s = "char *str = \"\";";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token2.len == 4);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token2.len == 1);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token2.len == 3);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token2.len == 1);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token2.len == 0);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token2.len == 1);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(token.s == NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);

	TESTA_ASSERT(prs_unparse(&ctx, &token) == 0);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 1);

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token2.len == 1);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_unparse(&ctx, &token2));
	TESTA_ASSERT(prs_unparse(&ctx, &token));

	TESTA_ASSERT(prs_parse(&ctx, &token2));
	TESTA_ASSERT(token2.s != NULL);
	TESTA_ASSERT(token2.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token2.len == 4);
	TESTA_ASSERT(token2.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(token.s != NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(*ctx.s != PRS_EOS);
	TESTA_ASSERT(ctx.line == 1);

	return 0;
}

static int token_compare(void)
{
	prs_context_t ctx;
	prs_token_t token;
	const char *s = "name = \"Leroy \\\"Jenkins\\\"\";";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_compare(&token, "name"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_compare(&token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_compare(&token, "Leroy \"Jenkins\""));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 17);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_compare(&token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(prs_token_compare(&token, ""));
	TESTA_ASSERT(prs_token_compare(&token, NULL));
	TESTA_ASSERT(token.s == NULL);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 1);

	return 0;
}

static int token_copy(void)
{
	prs_context_t ctx;
	prs_token_t token;
	unsigned int i;
	char buffer[16];
	const char *s = "name = \"Leroy \\\"Jenkins\\\"\";";

	prs_init(&ctx, s);

	for(i = 0; i < PRS_ARRAY_SIZE(buffer); i++)
		buffer[i] = 0x20;

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_copy(&token, buffer, PRS_ARRAY_SIZE(buffer)) == 4);

	TESTA_ASSERT(buffer[0] == 'n');
	TESTA_ASSERT(buffer[1] == 'a');
	TESTA_ASSERT(buffer[2] == 'm');
	TESTA_ASSERT(buffer[3] == 'e');
	TESTA_ASSERT(buffer[4] == PRS_EOS);
	TESTA_ASSERT(buffer[5] == 0x20);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_copy(&token, buffer, PRS_ARRAY_SIZE(buffer)) == 1);

	TESTA_ASSERT(buffer[0] == '=');
	TESTA_ASSERT(buffer[1] == PRS_EOS);
	TESTA_ASSERT(buffer[2] == 'm');
	TESTA_ASSERT(buffer[3] == 'e');
	TESTA_ASSERT(buffer[4] == PRS_EOS);
	TESTA_ASSERT(buffer[5] == 0x20);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_copy(&token, buffer, PRS_ARRAY_SIZE(buffer)) == 15);

	TESTA_ASSERT(buffer[0] == 'L');
	TESTA_ASSERT(buffer[1] == 'e');
	TESTA_ASSERT(buffer[2] == 'r');
	TESTA_ASSERT(buffer[3] == 'o');
	TESTA_ASSERT(buffer[4] == 'y');
	TESTA_ASSERT(buffer[5] == ' ');
	TESTA_ASSERT(buffer[6] == '"');
	TESTA_ASSERT(buffer[7] == 'J');
	TESTA_ASSERT(buffer[8] == 'e');
	TESTA_ASSERT(buffer[9] == 'n');
	TESTA_ASSERT(buffer[10] == 'k');
	TESTA_ASSERT(buffer[11] == 'i');
	TESTA_ASSERT(buffer[12] == 'n');
	TESTA_ASSERT(buffer[13] == 's');
	TESTA_ASSERT(buffer[14] == '"');
	TESTA_ASSERT(buffer[15] == PRS_EOS);

	TESTA_ASSERT(prs_parse(&ctx, &token));
	TESTA_ASSERT(prs_token_copy(&token, buffer, PRS_ARRAY_SIZE(buffer)) == 1);

	TESTA_ASSERT(buffer[0] == ';');
	TESTA_ASSERT(buffer[1] == PRS_EOS);
	TESTA_ASSERT(buffer[2] == 'r');
	TESTA_ASSERT(buffer[3] == 'o');
	TESTA_ASSERT(buffer[4] == 'y');
	TESTA_ASSERT(buffer[5] == ' ');
	TESTA_ASSERT(buffer[6] == '"');
	TESTA_ASSERT(buffer[7] == 'J');
	TESTA_ASSERT(buffer[8] == 'e');
	TESTA_ASSERT(buffer[9] == 'n');
	TESTA_ASSERT(buffer[10] == 'k');
	TESTA_ASSERT(buffer[11] == 'i');
	TESTA_ASSERT(buffer[12] == 'n');
	TESTA_ASSERT(buffer[13] == 's');
	TESTA_ASSERT(buffer[14] == '"');
	TESTA_ASSERT(buffer[15] == PRS_EOS);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(prs_token_copy(&token, buffer, PRS_ARRAY_SIZE(buffer)) == 0);

	TESTA_ASSERT(buffer[0] == PRS_EOS);
	TESTA_ASSERT(buffer[1] == PRS_EOS);
	TESTA_ASSERT(buffer[2] == 'r');
	TESTA_ASSERT(buffer[3] == 'o');
	TESTA_ASSERT(buffer[4] == 'y');
	TESTA_ASSERT(buffer[5] == ' ');
	TESTA_ASSERT(buffer[6] == '"');
	TESTA_ASSERT(buffer[7] == 'J');
	TESTA_ASSERT(buffer[8] == 'e');
	TESTA_ASSERT(buffer[9] == 'n');
	TESTA_ASSERT(buffer[10] == 'k');
	TESTA_ASSERT(buffer[11] == 'i');
	TESTA_ASSERT(buffer[12] == 'n');
	TESTA_ASSERT(buffer[13] == 's');
	TESTA_ASSERT(buffer[14] == '"');
	TESTA_ASSERT(buffer[15] == PRS_EOS);

	return 0;
}

static int token_type_to_str(void)
{
	prs_token_t token;

	token.type = PRS_TOKEN_TYPE_INVALID;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "invalid") == 0);

	token.type = PRS_TOKEN_TYPE_SYMBOL;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "symbol") == 0);

	token.type = PRS_TOKEN_TYPE_IDENTIFIER;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "identifier") == 0);

	token.type = PRS_TOKEN_TYPE_NUMBER;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "number") == 0);

	token.type = PRS_TOKEN_TYPE_STRING;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "string") == 0);

	token.type = 13;
	TESTA_ASSERT(strcmp(prs_token_type_to_str(&token), "unknown") == 0);

	return 0;
}

static int utf8(void)
{
	prs_context_t ctx;
	prs_token_t token;
	const char *s = "name = \"Лерой Дженкинс\";";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "name"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "Лерой Дженкинс"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 27);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ";"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);
	TESTA_ASSERT(token.s == NULL);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 1);

	return 0;
}

static int multiline(void)
{
	prs_context_t ctx;
	prs_token_t token;

	const char *s =
	"{\n"
	"\tconfig\t=\t\t\t\"\"\"hello\n"
	"\t\tw\\\"orld'\n"
	"\naweso\"m\\\'e\t\t\n"
	"\"\"\"\n\t"
	"}\n\n\n";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "{"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);
	TESTA_ASSERT(token.c == '{');

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "config"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 6);
	TESTA_ASSERT(token.line == 2);
	TESTA_ASSERT(token.c == 'c');

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "="));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);
	TESTA_ASSERT(token.c == '=');

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "hello\n\t\tw\"orld'\n\naweso\"m'e\t\t\n"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 31);
	TESTA_ASSERT(token.line == 2);
	TESTA_ASSERT(token.c == '"');

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "}"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 7);
	TESTA_ASSERT(token.c == '}');

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);
	TESTA_ASSERT(token.s == NULL);
	TESTA_ASSERT(token.c == 0);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 10);

	return 0;
}

static int json(void)
{
	prs_context_t ctx;
	prs_token_t token;

	const char *s =
	"{\n"
		"\t\"tags\": [\"zombies\", \"t\\\"esta\\\"z\"],\n"
		"\t\"autostart\":   \t    \n true,\n"
		"\t\"sleep\":\n\n   0.5,\n"
		"\t\"max\": \n13  \n"
	"}\n\n\n";

	prs_init(&ctx, s);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "{"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 1);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "tags"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ":"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "["));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "zombies"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 7);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ","));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "t\"esta\"z"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 10);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "]"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ","));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 2);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "autostart"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 9);
	TESTA_ASSERT(token.line == 3);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ":"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 3);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "true"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_IDENTIFIER);
	TESTA_ASSERT(token.len == 4);
	TESTA_ASSERT(token.line == 4);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ","));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 4);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "sleep"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 5);
	TESTA_ASSERT(token.line == 5);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ":"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 5);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "0.5"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 7);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ","));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 7);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "max"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_STRING);
	TESTA_ASSERT(token.len == 3);
	TESTA_ASSERT(token.line == 8);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, ":"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 8);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "13"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_NUMBER);
	TESTA_ASSERT(token.len == 2);
	TESTA_ASSERT(token.line == 9);

	TESTA_ASSERT(prs_parse_expect(&ctx, &token, "}"));
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_SYMBOL);
	TESTA_ASSERT(token.len == 1);
	TESTA_ASSERT(token.line == 10);

	TESTA_ASSERT(prs_parse(&ctx, &token) == 0);
	TESTA_ASSERT(token.type == PRS_TOKEN_TYPE_INVALID);
	TESTA_ASSERT(token.len == 0);
	TESTA_ASSERT(token.line == 0);
	TESTA_ASSERT(token.s == NULL);

	TESTA_ASSERT(*ctx.s == PRS_EOS);
	TESTA_ASSERT(ctx.line == 13);

	return 0;
}

TESTA_SUITE_BEGIN
	TESTA_GROUP_BEGIN("api")
		TESTA_TEST(init)
		TESTA_TEST(parse)
		TESTA_TEST(parse_expect)
		TESTA_TEST(unparse)
		TESTA_TEST(token_compare)
		TESTA_TEST(token_copy)
		TESTA_TEST(token_type_to_str)
	TESTA_GROUP_END
	TESTA_GROUP_BEGIN("utf-8")
		TESTA_TEST(utf8)
	TESTA_GROUP_END
	TESTA_GROUP_BEGIN("parse")
		TESTA_TEST(multiline)
		TESTA_TEST(json)
	TESTA_GROUP_END
TESTA_SUITE_END
