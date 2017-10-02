#include <string.h>
#include "UnitTest++/UnitTest++.h"
#include "privablic.h"

#include "../headers/bstring.h"

//

SUITE(BString)
{
	static char test_str1[] = "Hello, world!";
	static int  test_str1_length = 13;
	static char test_str2[] = "a✓♞☭€❄1♫";
	static int  test_str2_length = 8;
	
	TEST(EmptyConstructor)
	{
		BString *str = new BString();
		
		CHECK(! strcmp(str->c_str(), ""));
		CHECK_EQUAL(str->length(), 0);
		
		delete str;
	}
	
	TEST(ConstructorWithInitialString)
	{
		BString *str = new BString(test_str1);
		
		CHECK(! strcmp(str->c_str(), test_str1));
		CHECK_EQUAL(strlen(test_str1), str->length());
		
		delete str;
	}

	TEST(set)
	{
		BString *str = new BString(test_str1);
		
		str->set(test_str2);
		
		CHECK_EQUAL(strcmp(test_str2, str->c_str()), 0);
		
		delete str;
	}

	TEST(length)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(test_str1_length, str->length());
		
		delete str;
	}

	TEST(length_EmptyString)
	{
		BString *str = new BString();
		
		CHECK_EQUAL(0, str->length());
		
		delete str;
	}

	TEST(length_utf8)
	{
		BString *str = new BString(test_str2);
		
		CHECK_EQUAL(test_str2_length, str->length_utf8());
		
		delete str;
	}

	TEST(length_utf8_EmptyString)
	{
		BString *str = new BString();
		
		CHECK_EQUAL(0, str->length_utf8());
		
		delete str;
	}
	
	TEST(c_str)
	{
		BString *str = new BString(test_str2);
		
		CHECK(! strcmp(str->c_str(), test_str2));
		
		delete str;
	}
}
