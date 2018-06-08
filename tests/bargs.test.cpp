#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/args.h>

/* Privablic */
struct PRIV_find_by_name { typedef BArgs::Option *(*type)(char *name, size_t length, BArgs::Option *options); };
template class private_method<PRIV_find_by_name, &BArgs::find_by_name>;

struct PRIV_find_by_abbr { typedef BArgs::Option *(*type)(char abbr, BArgs::Option *options); };
template class private_method<PRIV_find_by_abbr, &BArgs::find_by_abbr>;

SUITE(BMemArray)
{
	struct BArgs::Option options_a[] = {
		{"strict",   0, (char **) 0x73747269},
		{"file",   'f', (char **) 0x66696c65},
		{NULL, 0, NULL}
	};

	struct BArgs::Option options_b[] = {
		{NULL, 0, NULL}
	};
	
	TEST (find_by_name)
	{
		CHECK((*func<PRIV_find_by_name>::ptr)("file", 4, options_a) == &options_a[1]);
		CHECK((*func<PRIV_find_by_name>::ptr)("file", 4, options_b) == NULL);
	}

	TEST (find_by_abbr)
	{
		CHECK((*func<PRIV_find_by_abbr>::ptr)('f', options_a) == &options_a[1]);
		CHECK((*func<PRIV_find_by_abbr>::ptr)('f', options_b) == NULL);
	}
	
	TEST (parse)
	{
		int   argc   = 4;
		char *argv[] = {"/bin/cat", "pos1", "--file=my--file=.txt=", "pos2"};
		
		struct
		{
			char *filename;
		} vals;
		
		struct BArgs::Option my_options[] = {
			//{"strict",   0, 0x73747269},
			{"file",   'f', &vals.filename},
			{NULL, 0, NULL}
		};
		
		BArgs::parse(argc, argv, my_options);
		
		CHECK_EQUAL(vals.filename, "my--file=.txt=");
	}
}
