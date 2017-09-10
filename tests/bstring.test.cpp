#include <string.h>
#include "UnitTest++/UnitTest++.h"

#include "../headers/bstring.h"

//

SUITE(BString)
{
	static char test_str1[] = "Hello, world!";
	static int  test_str1_length = 13;
	static char test_str2[] = "✓♞☭€❄1a♫";
	static int  test_str2_length = 8;

	TEST(EmptyConstructor)
	{
		BString *str = new BString();
		
		CHECK(! strcmp(str->c_str(), ""));
		CHECK_EQUAL(0u, str->length());
		
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
		
		CHECK_EQUAL(0, strcmp(test_str2, str->c_str()));
		//CHECK_EQUAL(test_str2_length + 1, str->allocated_size);		// <- **This is the sort of thing we need to test!** I just commented it out because outherwise GTest would complain because it's a private member and I don't have internet right now to check how to probe private members.
		
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
}
