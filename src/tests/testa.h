/* {{{ */
/*
	Copyright (c) 2017 - 2018, Mihail Szabolcs

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
#ifndef TESTA_H
#define TESTA_H

#ifdef TESTA_REPORT_ASSERTS
	static unsigned int __testa_asserts = 0;
	#define TESTA_REPORT_ASSERTS_UNUSED() (void)(__testa_asserts)
	#define TESTA_REPORT_ASSERTS_INC() __testa_asserts++
#else
	#define TESTA_REPORT_ASSERTS_UNUSED()
	#define TESTA_REPORT_ASSERTS_INC()
#endif

#ifndef TESTA_VERSION
	#define TESTA_VERSION 0x102
#endif

#if defined(_WIN32) || defined(TESTA_COLORLESS)
	#define TESTA_COLOR_RED		""
	#define TESTA_COLOR_CYAN	""
	#define TESTA_COLOR_YELLOW	""
	#define TESTA_COLOR_LIME	""
	#define TESTA_COLOR_WHITE	""
	#define TESTA_COLOR_BLUE	""
	#define TESTA_COLOR_END		""
#else
	#define TESTA_COLOR_RED		"\033[1;31m"
	#define TESTA_COLOR_CYAN	"\033[1;36m"
	#define TESTA_COLOR_YELLOW	"\033[1;33m"
	#define TESTA_COLOR_LIME	"\033[1;32m"
	#define TESTA_COLOR_WHITE	"\033[1;37m"
	#define TESTA_COLOR_BLUE	"\033[1;34m"
	#define TESTA_COLOR_END		"\033[0m"
#endif

#ifndef TESTA_PRINTF
	#include <stdio.h>
	#define TESTA_PRINTF printf
#endif

#ifndef TESTA_TIME
	#include <time.h>
	#define TESTA_TIME() time(NULL)
#endif

#ifndef TESTA_TIME_FORMAT
	#define TESTA_TIME_FORMAT "%ld"
#endif

#ifndef TESTA_TIME_TYPE
	#define TESTA_TIME_TYPE time_t
#endif

#ifndef TESTA_PRINT_INDENT
	#define TESTA_PRINT_INDENT(x, indent) \
		TESTA_PRINTF("%*s" TESTA_COLOR_BLUE "%s" TESTA_COLOR_END, indent, "", #x)
#endif

#ifndef TESTA_INDENT
	#define TESTA_INDENT 2
#endif

#ifndef TESTA_LINE
	#define TESTA_LINE __LINE__
#endif

#ifndef TESTA_FILE
	#define TESTA_FILENAME __FILE__
#endif

#ifndef TESTA_EXIT_SUCCESS
	#define TESTA_EXIT_SUCCESS 0
#endif

#ifndef TESTA_EXIT_FAILURE
	#define TESTA_EXIT_FAILURE 1
#endif

#ifndef TESTA_UNUSED
	#define TESTA_UNUSED(x) (void)(x)
#endif

#ifndef TESTA_PRINT_WELCOME
	#define TESTA_PRINT_WELCOME(argc, argv) \
		TESTA_PRINTF(TESTA_COLOR_WHITE "Running tests:\n\n" TESTA_COLOR_END)
#endif

#ifndef TESTA_PRINT_REPORT
	#ifdef TESTA_REPORT_ASSERTS
		#define TESTA_PRINT_REPORT(argc, argv, count, success, failure, skips, elapsed) \
				TESTA_PRINTF(TESTA_COLOR_WHITE "\n%u tests, %u passed, %u failures, %u skips, " \
							 "%u asserts, " TESTA_TIME_FORMAT " seconds\n" TESTA_COLOR_END, \
							 count, success, failure, skips, __testa_asserts, elapsed)
	#else
		#define TESTA_PRINT_REPORT(argc, argv, count, success, failure, skips, elapsed) \
				TESTA_PRINTF(TESTA_COLOR_WHITE "\n%u tests, %u passed, %u failures, %u skips, " \
							 TESTA_TIME_FORMAT " seconds\n" TESTA_COLOR_END, \
							 count, success, failure, skips, elapsed)
	#endif
#endif

#ifndef TESTA_PRINT_ASSERT
	#define TESTA_PRINT_ASSERT(x) \
		TESTA_PRINTF(TESTA_COLOR_RED " [FAILED] " TESTA_COLOR_CYAN "%s:%d %s\n" TESTA_COLOR_END, \
					 TESTA_FILENAME, TESTA_LINE, #x)
#endif

#ifndef TESTA_PRINT_SUCCESS
	#define TESTA_PRINT_SUCCESS(x, indent) \
		TESTA_PRINTF(TESTA_COLOR_LIME " [PASSED]\n" TESTA_COLOR_END)
#endif

#ifndef TESTA_PRINT_FAILURE
	#define TESTA_PRINT_FAILURE(x, indent)
#endif

#ifndef TESTA_PRINT_SKIP
	#define TESTA_PRINT_SKIP(x, indent) \
		TESTA_PRINTF(TESTA_COLOR_YELLOW " [SKIPPED]\n" TESTA_COLOR_END)
#endif

#define	TESTA_PRINT_GROUP(x) \
	TESTA_PRINTF("%*s " TESTA_COLOR_WHITE "%s\n" TESTA_COLOR_END, indent, "", x)

#define TESTA_RESULT_SUCCESS 0
#define TESTA_RESULT_FAILURE 1
#define TESTA_RESULT_SKIP 2

#define TESTA_SKIP() \
		return TESTA_RESULT_SKIP

#define TESTA_ASSERT(x) \
	do \
	{ \
		TESTA_REPORT_ASSERTS_INC(); \
		if(!(x)) \
		{ \
			TESTA_PRINT_ASSERT(x); \
			return TESTA_RESULT_FAILURE; \
		} \
	} while(0)

#define TESTA_ASSERT_EQUAL(x, y) TESTA_ASSERT(x == y)
#define TESTA_ASSERT_NOT_EQUAL(x, y) TESTA_ASSERT(x != y)

#define TESTA_SUITE_BEGIN \
	int main(int argc, char *argv[]) \
	{ \
		unsigned int count = 0, success = 0, failure = 0, skips = 0, indent = 0; \
		TESTA_TIME_TYPE started; \
		TESTA_REPORT_ASSERTS_UNUSED(); \
		TESTA_UNUSED(argc); \
		TESTA_UNUSED(argv); \
		TESTA_UNUSED(count); \
		TESTA_UNUSED(success); \
		TESTA_UNUSED(failure); \
		TESTA_UNUSED(skips); \
		TESTA_UNUSED(indent); \
		TESTA_UNUSED(started); \
		TESTA_PRINT_WELCOME(argc, argv); \
		started = TESTA_TIME(); \
		{
		
#define TESTA_SUITE_END \
		} \
		TESTA_PRINT_REPORT(argc, argv, count, success, failure, skips, TESTA_TIME() - started); \
		return failure > 0 ? TESTA_EXIT_FAILURE : TESTA_EXIT_SUCCESS; \
	}

#define TESTA_GROUP_BEGIN(s) \
	{ \
		TESTA_PRINT_GROUP(s); \
		indent += TESTA_INDENT;

#define TESTA_GROUP_END \
		indent -= TESTA_INDENT; \
	}

#define TESTA_TEST(x) \
	do \
	{ \
		count++; \
		TESTA_PRINT_INDENT(x, indent); \
		switch(x()) \
		{ \
			case TESTA_RESULT_SUCCESS: \
			{ \
				success++; \
				TESTA_PRINT_SUCCESS(x, indent); \
			} \
			break; \
			case TESTA_RESULT_FAILURE: \
			{ \
				failure++; \
				TESTA_PRINT_FAILURE(x, indent); \
			} \
			break; \
			case TESTA_RESULT_SKIP: \
			{ \
				skips++; \
				TESTA_PRINT_SKIP(x, indent); \
			} \
			break; \
		} \
	} while(0);

#endif

/* vim: set ts=4 sw=4 sts=4 noet: */
