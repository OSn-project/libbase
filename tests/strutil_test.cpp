#include <stdio.h>
#include <string.h>
#include "UnitTest++/UnitTest++.h"

#include "../include/base/strutil.h"
#include "../include/base/dict.h"

SUITE (BStrUtil)
{
	TEST (fill_templ)
	{
		BDict<const char *> info;
		info.add("user_name", "robert");
		info.add("user_age", "5");
		info.add("", "empty");

		BString str1("My name $(is $(user_name)) and $(I'm $(user_age).");
		BStrUtil::fill_templ(&str1, "$(", ")", &info);
		CHECK(str1.equals("My name $(is robert) and $(I'm 5."));
		CHECK(str1.size() == 33);

		BString str2("User name: |user_name|, age: |user_age|");
		BStrUtil::fill_templ(&str2, "|", "|", &info);
		CHECK(str2.equals("User name: robert, age: 5"));

		BString str3("{user_age} {nonexistent} {user_age}");
		bool rc = BStrUtil::fill_templ(&str3, "{", "}", &info);
		CHECK(rc == false); // 'nonexistent' not in dictionary.
		CHECK(str3.equals("{user_age} {nonexistent} {user_age}"));

		BString str4("||");
		BStrUtil::fill_templ(&str4, "|", "|", &info);
		CHECK(str4.equals("empty"));

		BString str5("");
		BStrUtil::fill_templ(&str5, "", "", &info);
		CHECK(str5.equals(""));
}
}
