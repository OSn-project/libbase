#include <stdio.h>
#include <string.h>
#include "UnitTest++/UnitTest++.h"

#include "../include/base/strutil.h"
#include "../include/base/dict.h"

SUITE (BStrUtil)
{
	TEST (fill_templ)
	{
		BDict info;
		info.add("user_name", (void *) "robert");
		info.add("user_age", (void *) "5");
		info.add("", (void *) "empty");

		BString str1("My name $(is $(user_name)) and $(I'm $(user_age).");
		BStrUtil::fill_templ(&str1, &info, "$(", ")");
		CHECK(str1.equals("My name $(is robert) and $(I'm 5."));
		CHECK(str1.size() == 33);

		BString str2("User name: |user_name|, age: |user_age|");
		BStrUtil::fill_templ(&str2, &info, "|", "|");
		CHECK(str2.equals("User name: robert, age: 5"));

		BString str3("{}");
		BStrUtil::fill_templ(&str3, &info, "{", "}");
		CHECK(str3.equals("empty"));
	}
}
