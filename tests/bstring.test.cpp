#include <stdio.h>
#include <string.h>

#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "UnitTest++/UnitTest++.h"
#include "privablic.h"

#include "../headers/bstring.h"
#include "../headers/utf8.h"

//

struct BString_m_size { typedef size_t (BString::*type); };
template class private_member<BString_m_size, &BString::m_size>;

SUITE (BString)
{
	static char test_str1[] = "Hello, world!";
	static int  test_str1_length = 13;
	static char test_str2[] = "a✓♞☭€❄1♫";
	static int  test_str2_length = 8;
	
	TEST (EmptyConstructor)
	{
		BString *str = new BString();
		
		REQUIRE CHECK(! strcmp(str->c_str(), ""));
		REQUIRE CHECK(str->*member<BString_m_size>::value == 0);
		
		delete str;
	}
	
	TEST (ConstructorWithInitialString)
	{
		BString *str = new BString(test_str1);
		
		REQUIRE CHECK(! strcmp(str->c_str(), test_str1));
		REQUIRE CHECK(str->*member<BString_m_size>::value == test_str1_length);
		CHECK(str->utf8 == false);
		
		delete str;
	}

	TEST (ConstructorWithInitialStringUTF8)
	{
		BString *str = new BString(test_str2, true);
		
		REQUIRE CHECK(! strcmp(str->c_str(), test_str2));
		REQUIRE CHECK(str->*member<BString_m_size>::value == sizeof(test_str2) - 1);
		CHECK(str->utf8 == true);
		
		delete str;
	}

	TEST (set)
	{
		BString *str = new BString(test_str1);
		
		bool s = str->set(test_str2);
		
		CHECK(s == true);
		CHECK_EQUAL(str->c_str(), test_str2);
		CHECK(str->*member<BString_m_size>::value == sizeof(test_str2) - 1);
		
		delete str;
	}

	TEST (set_MallocReturnsNull)
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

	TEST (length)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(test_str1_length, str->length());
		
		delete str;
	}

	TEST (length_EmptyString)
	{
		BString *str = new BString();
		
		CHECK_EQUAL(0, str->length());
		
		delete str;
	}

	TEST (length_UTF8)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK_EQUAL(test_str2_length, str->length());
		
		delete str;
	}

	TEST (length_UTF8EmptyString)
	{
		BString *str = new BString();
		str->utf8 = true;
		
		CHECK_EQUAL(str->length(), 0);
		
		delete str;
	}

	TEST (utf8_size)
	{
		BString *str = new BString(test_str2);
		
		CHECK_EQUAL(6, str->utf8_size(3, 5));
		
		delete str;
	}

	TEST (utf8_size_OutOfBounds)
	{
		BString *str = new BString(test_str2);
		
		CHECK_EQUAL(16, str->utf8_size(2, 15));
		CHECK_EQUAL( 0, str->utf8_size(-2, 3));
		CHECK_EQUAL( 0, str->utf8_size( 5, 1));
		
		delete str;
	}
	
	TEST (append)	// Note: we don't test inlines because they're just wrappers around the main function
	{
		BString *str = new BString(test_str1);
		
		str->append(test_str2, sizeof(test_str2) - 1);
		
		CHECK_EQUAL("Hello, world!a✓♞☭€❄1♫", str->c_str());
		CHECK_EQUAL(str->*member<BString_m_size>::value, sizeof(test_str1) + sizeof(test_str2) - 1);		// Don't ask me how privablic works but it does magic and that's what I need.
	}

	TEST (prepend)	// Note: we don't test inlines because they're just wrappers around the main function
	{
		BString *str = new BString(test_str2);
		
		str->prepend(test_str1, sizeof(test_str1) - 1);
		
		CHECK_EQUAL("Hello, world!a✓♞☭€❄1♫", str->c_str());
		CHECK_EQUAL(str->*member<BString_m_size>::value, sizeof(test_str2) + sizeof(test_str1) - 1);		// Don't ask me how privablic works but it does magic and that's what I need.
	}
	
	TEST (uppercase)
	{
		BString *str   = new BString (test_str1);
		BString *upper = str->uppercase();
		
		CHECK_EQUAL("HELLO, WORLD!", upper->c_str());
		
		delete str;
		delete upper;
	}

	TEST (uppercase_2)
	{
		BString *str   = new BString (test_str2, true);
		BString *upper = str->uppercase();
		
		CHECK_EQUAL("A✓♞☭€❄1♫", upper->c_str());
		
		delete str;
		delete upper;
	}

	TEST (lowercase)
	{
		BString *str   = new BString (test_str1);
		BString *lower = str->lowercase();
		
		CHECK_EQUAL("hello, world!", lower->c_str());
		
		delete str;
		delete lower;
	}

	TEST (lowercase_2)
	{
		BString *str   = new BString (test_str2, true);
		BString *lower = str->lowercase();
		
		CHECK_EQUAL("a✓♞☭€❄1♫", lower->c_str());
		
		delete str;
		delete lower;
	}
	
	TEST (count)
	{
		BString *str = new BString ("abcabcc");
		
		CHECK(str->count('c') == 3);
		CHECK(str->count('z') == 0);
		
		delete str;
	}

	TEST (count_EmptyString)
	{
		BString *str = new BString ();
		
		CHECK(str->count('x') == 0);
		
		delete str;
	}
	
	TEST (count_chars)
	{
		BString *str = new BString ("abcabcc♞♞❄♞♫");
		
		CHECK(str->count_chars("cbz") == 5);
		CHECK(str->count_chars("")    == 0);
//		CHECK(str->count_chars("cc")  == 6);	// Extra goodie! But it's unintended and not required so we don't test it.
		
		delete str;
	}

	TEST (count_chars_UTF8)
	{
		BString *str = new BString ("abcabcc♞♞❄♞♫", true);
		
		CHECK(str->count_chars("cb♞z") == 8);
		CHECK(str->count_chars("")    == 0);
		
		delete str;
	}
	
	TEST (char_at)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(*(str->char_at(4)), 'o');
	}

	TEST (char_at_OutOfBounds)
	{
		BString *str = new BString(test_str1);
		
		CHECK(str->char_at(16) == "");
	}

	TEST (char_at_Negative)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(*(str->char_at(-1)), '!');
	}

	TEST (char_at_NegativeOutOfBounds)
	{
		BString *str = new BString(test_str1);
		
		CHECK(str->char_at(-44) == "");
	}

	TEST (char_at_UTF8)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(utf8_charcmp(str->char_at(4), "€"));
	}

	TEST (char_at_UF8_OutOfBounds)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(str->char_at(16) == "");
	}

	TEST (char_at_UF8_Negative)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(utf8_charcmp(str->char_at(-1), "♫"));
	}

	TEST (char_at_UF8_NegativeOutOfBounds)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(str->char_at(-44) == "");
	}
	
	TEST (c_str)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(! strcmp(str->c_str(), test_str2));
		
		delete str;
	}
}
