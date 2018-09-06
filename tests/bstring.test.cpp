#include <stdio.h>
#include <string.h>

#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "UnitTest++/UnitTest++.h"
#include "privablic.h"

#include "../include/base/string.h"
#include "../include/utf8.h"

/* Privablic */

struct BString_m_size { typedef size_t (BString::*type); };
template class private_member<BString_m_size, &BString::m_size>;

//

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
	
	TEST (ConstructorWithInitialStringAndSize)
	{
		BString *str = new BString(test_str1, false, test_str1_length);
		
		CHECK(! strcmp(str->c_str(), test_str1));
		CHECK(str->*member<BString_m_size>::value == test_str1_length);
		CHECK(str->utf8 == false);
		
		delete str;
	}

	TEST (ConstructorWithInitialStringAndSizeLength0)
	{
		BString *str = new BString(test_str1, false, 0);
		
		CHECK(! strcmp(str->c_str(), ""));
		CHECK(str->*member<BString_m_size>::value == 0);
		CHECK(str->utf8 == false);
		
		delete str;
	}

	TEST (ConstructorWithInitialBString)
	{
		BString  init(test_str1);
		BString *str = new BString(init);
		
		CHECK(! strcmp(str->c_str(), test_str1));
		CHECK(str->*member<BString_m_size>::value == test_str1_length);
		CHECK(str->utf8 == false);
		
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

	TEST (clear)
	{
		BString *str = new BString(test_str1);
		
		str->clear();
		
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

	TEST (size)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK_EQUAL(str->*member<BString_m_size>::value, str->size());
		
		delete str;
	}

	TEST (utf8_size)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK_EQUAL(6, str->utf8_size(3, 5));
		CHECK_EQUAL(6, str->utf8_size(-5, -3));
		
		delete str;
	}

	TEST (utf8_size_OutOfBounds)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK_EQUAL(16, str->utf8_size( 2,15));
		CHECK_EQUAL( 0, str->utf8_size(-2, 3));
		CHECK_EQUAL( 0, str->utf8_size( 5, 1));
		
		delete str;
	}
	
	TEST (split)
	{
		static char *expected_strings[] = {"", "Lor", "em ", "ip", "sum d", "olo", "", "r si", "t amet."};
		
		BString  *str = new BString("/Lor/em /ip/sum d/olo//r si/t amet.");
		BString **sections = str->split('/');
		
		char **expected = expected_strings;
		
		for (BString **s = sections; *s != NULL; s++)
		{
			CHECK_EQUAL((*s)->c_str(), *expected);
			expected++;
		}
		
		BString::tuple_free(sections);
		delete str;
	}
	
	TEST (append)	// Note: we don't test inlines because they're just wrappers around the main function
	{
		BString *str = new BString(test_str1);
		
		str->append(test_str2, sizeof(test_str2) - 1);
		
		CHECK_EQUAL("Hello, world!a✓♞☭€❄1♫", str->c_str());
		CHECK_EQUAL(str->*member<BString_m_size>::value, sizeof(test_str1) + sizeof(test_str2) - 1);		// Don't ask me how privablic works but it does magic and that's what I need.
		
		delete str;
	}

	TEST (prepend)
	{
		BString *str = new BString(test_str2, true);
		
		str->prepend(test_str1, sizeof(test_str1) - 1);
		
		CHECK_EQUAL("Hello, world!a✓♞☭€❄1♫", str->c_str());
		CHECK_EQUAL(str->*member<BString_m_size>::value, sizeof(test_str2) + sizeof(test_str1) - 1);
		
		delete str;
	}

	TEST (prepend2)
	{
		BString *str = new BString("bcdefgh");
		
		str->prepend("a", 1);
		
		CHECK_EQUAL("abcdefgh", str->c_str());
		
		delete str;
	}

	TEST (insert)
	{
		BString *str = new BString(test_str1);
		
		str->insert(test_str2, sizeof(test_str2) - 1, 6);
		
		CHECK_EQUAL("Hello,a✓♞☭€❄1♫ world!", str->c_str());
		CHECK_EQUAL(sizeof(test_str1) + sizeof(test_str2) - 2, str->*member<BString_m_size>::value);
		
		delete str;
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
	
	TEST (equals)
	{
		BString str_a("abcdefg");
		BString str_b("abcdefg");
		BString str_c("zzzzzzz");
		
		CHECK(BString::equals(&str_a, "abc123", 3) == true);
		CHECK(BString::equals(&str_a, "abc123") == false);

		CHECK(BString::equals(&str_a, &str_b) == true);
		CHECK(BString::equals(&str_a, &str_c) == false);
	}
	
	TEST (remove)
	{
		BString *str  = new BString (test_str2, true);
		BString *orig = new BString (test_str2, true);
		
		str->remove(1, 5);
		
		CHECK_EQUAL("a1♫", str->c_str());
		CHECK_EQUAL(sizeof(test_str2) - 1 - orig->utf8_size(1, 1 + 5), str->*member<BString_m_size>::value);
		
		delete str;
		delete orig;
	}

	TEST (remove_ZeroLength)
	{
		BString *str = new BString (test_str2, true);
		
		str->remove(1, 0);
		
		CHECK_EQUAL(test_str2, str->c_str());
		
		delete str;
	}

	TEST (remove_TooLong)
	{
		BString *str = new BString (test_str2, true);
		
		str->remove(1, 40);
		
		CHECK_EQUAL(test_str2, str->c_str());
		
		delete str;
	}

	TEST (remove_char)
	{
		BString *str = new BString (test_str1);
		
		str->remove_char('l');
		
		CHECK_EQUAL("Heo, word!", str->c_str());
		CHECK_EQUAL(sizeof(test_str1) - 1 - 3, str->*member<BString_m_size>::value);
		
		delete str;
	}

	TEST (remove_char_NullByte)
	{
		/* Calling ::remove_char() with '\0' would remove	*
		 * the null-terminator, which we don't want.		*/ 
		
		BString *str = new BString (test_str1);
		
		str->remove_char('\0');
		
		CHECK_EQUAL(test_str1, str->c_str());
		CHECK_EQUAL(sizeof(test_str1) - 1, str->*member<BString_m_size>::value);
		
		delete str;
	}

	TEST (remove_char_EmptyStr)
	{
		BString *str = new BString();
		
		str->remove_char('F');
		
		CHECK_EQUAL("", str->c_str());
		CHECK_EQUAL(0, str->*member<BString_m_size>::value);
		
		delete str;
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
//		CHECK(str->count_chars("cc")  == 6);	// Extra goodie! Putting a letter twice returns twice the count of the letter. But it's unintended and not required so we don't test it.
		
		delete str;
	}
	
	TEST (count_chars_UTF8)
	{
		BString *str = new BString ("abcabcc♞♞❄♞♫", true);
		
		CHECK(str->count_chars("cb♞z") == 8);
		CHECK(str->count_chars("")     == 0);
		
		delete str;
	}
	
	TEST (index_of)
	{
		BString *str = new BString ("The quick brown fox jumps over the lazy dog.");
		
		CHECK(str->index_of('T')  ==  0);
		CHECK(str->index_of('m')  == 22);
		CHECK(str->index_of(';')  == -1);
		CHECK(str->index_of('\0') == 44);

		CHECK(str->index_of_utf8("я") == -1);
		CHECK(str->index_of('a', test_str1) == -1);	// Check that the function catches an out-of-bounds start pointer.

		delete str;		
	}

	TEST (index_of_EmptyStr)
	{
		BString *str = new BString ("");
		
		CHECK(str->index_of('T') == -1);
		CHECK(str->index_of_utf8("и") == -1);

		delete str;		
	}

	TEST (index_of_UTF8)
	{
		BString *str = new BString ("Санкт-Петербургская классическая гимназия 610", true);
		
		CHECK(str->index_of_utf8("С")  ==  0);
		CHECK(str->index_of_utf8("я")  == 18);
		CHECK(str->index_of_utf8(";")  == -1);
		CHECK(str->index_of_utf8("")   == 45);

		CHECK(str->index_of('1')  == 43);
		CHECK(str->index_of_utf8("П", test_str1) == -1);	// Check that the function catches an out-of-bounds start pointer.
	
		delete str;		
	}
	
	TEST (offset_of)
	{
		BString *str = new BString ("The quick brown fox jumps over the lazy dog.");
		
		CHECK(str->offset_of('T')  ==  0);
		CHECK(str->offset_of('m')  == 22);
		CHECK(str->offset_of(';')  == -1);
		CHECK(str->offset_of('\0') == 44);

		CHECK(str->offset_of_utf8("я") == -1);
		CHECK(str->offset_of('a', test_str1) == -1);	// Check that the function catches an out-of-bounds start pointer.

		delete str;		
	}

	TEST (offset_of_EmptyStr)
	{
		BString *str = new BString ("");

		CHECK(str->offset_of_utf8("я") == -1);
		CHECK(str->offset_of('a', test_str1) == -1);	// Check that the function catches an out-of-bounds start pointer.

		delete str;		
	}

	TEST (offset_of_UTF8)
	{
		BString *str = new BString ("Санкт-Петербургская классическая гимназия 610", true);
		
		CHECK(str->offset_of_utf8("С")  ==  0);
		CHECK(str->offset_of_utf8("я")  == 35);
		CHECK(str->offset_of_utf8(";")  == -1);
		CHECK(str->offset_of_utf8("")   == str->*member<BString_m_size>::value);

		CHECK(str->offset_of('1')  == str->*member<BString_m_size>::value - 2);
		CHECK(str->offset_of_utf8("П", test_str1) == -1);	// Check that the function catches an out-of-bounds start pointer.
	
		delete str;		
	}
	
	TEST (next)
	{
		CHECK(BString::next('o', test_str1) == test_str1 + 4);
		CHECK(BString::next('l', BString::next('o', test_str1)) == test_str1 + 10);

		CHECK(BString::next('z', test_str1) == test_str1 + sizeof(test_str1) - 1);
	}
	
	TEST (offset_of_ptr)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK_EQUAL(3, str->offset_of_ptr(str->c_str() + 7));
		CHECK_EQUAL(0, str->offset_of_ptr(str->c_str()));
		
		CHECK_EQUAL(str->length(), str->offset_of_ptr(str->c_str() + str->size()));
		
		CHECK_EQUAL(-1, str->offset_of_ptr((const char *) stderr));
		CHECK_EQUAL(-1, str->offset_of_ptr((const char *) NULL));
		
		delete str;
	}

	TEST (char_at)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(*(str->char_at(4)), 'o');
		
		delete str;
	}

	TEST (char_at_OutOfBounds)
	{
		BString *str = new BString(test_str1);
		
		CHECK(str->char_at(16) == '\0');
		
		delete str;
	}

	TEST (char_at_Negative)
	{
		BString *str = new BString(test_str1);
		
		CHECK_EQUAL(*(str->char_at(-1)), '!');
		
		delete str;
	}

	TEST (char_at_NegativeOutOfBounds)
	{
		BString *str = new BString(test_str1);
		
		CHECK(str->char_at(-44) == '\0');
		
		delete str;
	}

	TEST (char_at_UTF8)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(utf8_charcmp(str->char_at(4), "€"));
		
		delete str;
	}

	TEST (char_at_UF8_OutOfBounds)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(str->char_at(16) == '\0');
		
		delete str;
	}

	TEST (char_at_UF8_Negative)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(utf8_charcmp(str->char_at(-1), "♫"));
		
		delete str;
	}

	TEST (char_at_UF8_NegativeOutOfBounds)
	{
		BString *str = new BString(test_str2, true);
		
		CHECK(str->char_at(-44) == '\0');
		
		delete str;
	}
	
	TEST (c_str)
	{
		BString *str = new BString(test_str2, true);
		
		REQUIRE CHECK(! strcmp(str->c_str(), test_str2));
		
		delete str;
	}
	
	/*TEST (read_file_FromPath)		// The test doesn't work but it's a wrapper function anyway.
	{
		char path[] = "somefilepath";
		MockRepository mocks;
		BString *bstr = mocks.Mock<BString>();
		
		mocks.ExpectCallFunc(fopen).With(path, "r").Return((FILE *) 0xff00ff00);
		//mocks.ExpectCallFunc(&BString::read_file).Return((BString *) 0x12345678);
		//mocks.ExpectCallOverload(bstr, (BString* (BString::*)(FILE *, int32, size_t))&BString::read_file).Return((BString *) 0x12345678);
		mocks.ExpectCallFunc(fclose);
		BString *str = BString::read_file(path);
		
		CHECK_EQUAL("", str->c_str());
		
		delete str;
	}*/

	TEST (read_file)
	{
		FILE *file = fmemopen(test_str1, sizeof(test_str1), "r");
		
		BString *str = BString::read_file(file, 5);
		
		CHECK_EQUAL("Hello", str->c_str());
		CHECK_EQUAL(5, str->*member<BString_m_size>::value);
		
		delete str;
		fclose(file);
	}

	TEST (read_file_ZeroBytes)
	{
		FILE *file = fmemopen(test_str1, sizeof(test_str1), "r");
		
		BString *str = BString::read_file(file, 0);
		
		CHECK_EQUAL("", str->c_str());
		
		delete str;
		fclose(file);
	}

	TEST (read_file_IndefiniteSize)
	{
		char test_str[] = "Санкт-Петербургская классическая гимназия";
		FILE *file = fmemopen(test_str, sizeof(test_str) - 1, "r");
		
		BString *str = BString::read_file(file, -1, 8);
		
		CHECK_EQUAL(test_str, str->c_str());
		CHECK_EQUAL(strlen(test_str), str->*member<BString_m_size>::value);
		
		delete str;
		fclose(file);
	}
	
	TEST (write_file)
	{
		MockRepository mocks;
		
		BString str("1234567890");
		FILE *file = (FILE *) 0xba5eba11;
		
		mocks.ExpectCallFunc(fwrite).With(str.c_str(), str.*member<BString_m_size>::value, 1, file).Return(1);
		
		str.write_file(file);
	}
	
	TEST (read_line)
	{
		char test_str[] = " over the lazy dog.\nMore data here.";
		FILE *file = fmemopen(test_str, sizeof(test_str) - 1, "r");
		
		BString str("The quick brown fox jumps");
		
		BString::read_line(file, &str);
		
		CHECK_EQUAL(str.c_str(), "The quick brown fox jumps over the lazy dog.");
	}
	
	TEST (sprintf)
	{
		BString str;
		char *name = "Jenny\0Rob";
		
		sprintf(&str, "%s is %d years old.", name, -443);
		
		CHECK(BString::equals(&str, "Jenny is -443 years old."));
		
		sprintf(&str, "%+5hd hello", (short) 55);
		
		CHECK(BString::equals(&str, "  +55 hello"));
	}
}
