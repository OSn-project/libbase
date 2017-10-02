#include <stdio.h>
#include <string.h>
#include "UnitTest++/UnitTest++.h"

#include "../headers/utf8.h"

SUITE (utf8)
{
	static char test_str1[] = "a✓♞☭€❄1♫";
	
	TEST(utf8_length)
	{
		CHECK(utf8_length(NULL) == 0);
		CHECK(utf8_length("")   == 0);
		CHECK(utf8_length(test_str1) == 8);
	}
	
	TEST(utf8_nextchar)
	{
		CHECK(test_str1 + 1 == utf8_nextchar(test_str1));
		CHECK(utf8_nextchar(test_str1 + 1) == test_str1 + 4);
	}
	
	TEST(utf8_char_at)
	{
		CHECK(utf8_char_at(test_str1,-1) == NULL);
		CHECK(utf8_char_at(test_str1, 0) == test_str1);
		CHECK(utf8_char_at(test_str1, 1) == test_str1 + 1);
		CHECK(utf8_char_at(test_str1, 4) == test_str1 + 10);
		CHECK(utf8_char_at(test_str1, 8) == test_str1 + (sizeof(test_str1) - 1));
		CHECK(utf8_char_at(test_str1, 9) == test_str1 + (sizeof(test_str1) - 1));
	}

	TEST(utf8_first)
	{
		char out[64];
		
		utf8_first(test_str1, out, 4);
		
		CHECK_EQUAL(out, "a✓♞☭");
	}

	TEST(utf8_first_NonPositiveLength)
	{
		char out[64];
		memset(out, 0xff, 64);
		
		utf8_first(test_str1, out, 0);
		CHECK(! strcmp(out, ""));

		utf8_first(test_str1, out, -16);
		CHECK(! strcmp(out, ""));
	}
}
