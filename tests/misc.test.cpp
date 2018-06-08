#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>

#include <base/misc.h>

SUITE(misc)
{
	TEST (streq)
	{
		CHECK(streq("abcd", "abcd") == true);
		CHECK(streq("abc", "abcd") == false);
	}
	
	TEST (strneq)
	{
		CHECK(strneq("abcdefg", "abcz", 3) == true);
		CHECK(strneq("abc", "abc", 64) == true);
	}
}
