#include <stdio.h>
#include <string.h>
#include "UnitTest++/UnitTest++.h"

#include "../include/utf8.h"

SUITE (utf8)
{
	static char test_str1[] = "a✓♞☭€❄1♫";
	
	TEST (utf8_charcmp)
	{
		/*REQUIRE*/ CHECK(utf8_charcmp(&test_str1[1], "✓") == true);
		/*REQUIRE*/ CHECK(utf8_charcmp("♞", "a") == false);
		/*REQUIRE*/ CHECK(utf8_charcmp("", "hello") == false);
	}
	
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

	TEST(utf8_slice)
	{
		char out[64];
		
		utf8_slice(test_str1, out, 0, 4); 
		CHECK_EQUAL(out, "a✓♞☭");

		utf8_slice(test_str1, out, 5, 3); 
		CHECK_EQUAL(out, "❄1♫");

		utf8_slice(test_str1, out, 2, 10); 
		CHECK_EQUAL(out, "♞☭€❄1♫");
	}

	TEST(utf8_slice_NonPositiveLength)
	{
		char out[64];
		
		memset(out, 0xff, 64);
		utf8_slice(test_str1, out, 0, 0);
		CHECK_EQUAL(out, "");

		memset(out, 0xff, 64);
		utf8_slice(test_str1, out, 0, -16);
		CHECK_EQUAL(out, "");
	}

	TEST(utf8_slice_NegativeStart)
	{
		char out[64];
		
		memset(out, 0xff, 64);
		utf8_slice(test_str1, out, -1, 3);
		CHECK_EQUAL(out, "");
	}

	TEST(strchr_utf8)
	{
		char *str = "€€€€€♫€€";

		CHECK(strchr_utf8(str, "♫") == str + 15);
		CHECK(strchr_utf8(str, "") == NULL);
		CHECK(strchr_utf8(str, "❄") == NULL);
	}
}
