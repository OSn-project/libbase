#include <stdio.h>
#include <string.h>

#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "UnitTest++/UnitTest++.h"
#include "privablic.h"

#include "../headers/bstring.h"

//

struct BString_m_size { typedef size_t (BString::*type); };
template class private_member<BString_m_size, &BString::m_size>;

SUITE(BString)
{
	static char test_str1[] = "Hello, world!";
	static int  test_str1_length = 13;
	static char test_str2[] = "a✓♞☭€❄1♫";
	static int  test_str2_length = 8;
	
	TEST(EmptyConstructor)
	{
		BString *str = new BString();
		
		REQUIRE CHECK(! strcmp(str->c_str(), ""));
		REQUIRE CHECK(str->*member<BString_m_size>::value == 0);
		
		delete str;
	}
	
	TEST(ConstructorWithInitialString)
	{
		BString *str = new BString(test_str1);
		
		REQUIRE CHECK(! strcmp(str->c_str(), test_str1));
		REQUIRE CHECK(str->*member<BString_m_size>::value == test_str1_length);
		
		delete str;
	}

	TEST(set)
	{
		BString *str = new BString(test_str1);
		
		bool s = str->set(test_str2);
		
		CHECK(s == true);
		CHECK_EQUAL(str->c_str(), test_str2);
		CHECK(str->*member<BString_m_size>::value == sizeof(test_str2) - 1);
		
		delete str;
	}

	TEST(set_MallocReturnsNull)
	{
		MockRepository mocks;
		
		BString *str = new BString(test_str1);
		
		mocks.ExpectCallFunc(realloc).Return(NULL);
		bool s = str->set(test_str2);
		
		CHECK(s == false);
		CHECK_EQUAL(str->c_str(), "");
		CHECK(str->*member<BString_m_size>::value == 0);
		
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
		
		CHECK_EQUAL(str->length_utf8(), 0);
		
		delete str;
	}

	TEST(utf8_size)
	{
		BString *str = new BString(test_str2);
		
		CHECK_EQUAL(6, str->utf8_size(3, 5));
		
		delete str;
	}

	TEST(utf8_size_OutOfBounds)
	{
		BString *str = new BString(test_str2);
		
		CHECK_EQUAL(16, str->utf8_size(2, 15));
		CHECK_EQUAL( 0, str->utf8_size(-2, 3));
		CHECK_EQUAL( 0, str->utf8_size( 5, 1));
		
		delete str;
	}
	
	TEST(uppercase)
	{
		BString *str   = new BString (test_str1);
		BString *upper = str->uppercase();
		
		CHECK_EQUAL("HELLO, WORLD!", upper->c_str());
		
		delete str;
		delete upper;
	}

	TEST(uppercase_2)
	{
		BString *str   = new BString (test_str2);
		BString *upper = str->uppercase();
		
		CHECK_EQUAL("A✓♞☭€❄1♫", upper->c_str());
		
		delete str;
		delete upper;
	}

	TEST(lowercase)
	{
		BString *str   = new BString (test_str1);
		BString *lower = str->lowercase();
		
		CHECK_EQUAL("hello, world!", lower->c_str());
		
		delete str;
		delete lower;
	}

	TEST(lowercase_2)
	{
		BString *str   = new BString (test_str2);
		BString *lower = str->lowercase();
		
		CHECK_EQUAL("a✓♞☭€❄1♫", lower->c_str());
		
		delete str;
		delete lower;
	}
	
	TEST(c_str)
	{
		BString *str = new BString(test_str2);
		
		CHECK(! strcmp(str->c_str(), test_str2));
		
		delete str;
	}
}
