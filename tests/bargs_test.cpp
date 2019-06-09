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

SUITE(BMemArray)
{
	struct BArgs::Option options_a[] = {
		{"strict", (char **) 0x73747269},
		{"output", (char **) 0x66696c65},
		{NULL, NULL}
	};

	struct BArgs::Option options_b[] = {
		{NULL, NULL}
	};
	
	TEST (find_by_name)
	{
		CHECK((*func<PRIV_find_by_name>::ptr)("output", 4, options_a) == &options_a[1]);
		CHECK((*func<PRIV_find_by_name>::ptr)("output", 4, options_b) == NULL);
	}
	
	TEST (parse)
	{
		int   argc   = 5;
		char *argv[] = {"/bin/cat", "pos1", "-file", "-my--file=.txt=", "-unknown", "-sizeeecheckthatitdoesntjustcomparethebeginning", "5M"};
		
		struct
		{
			char *filename;
			char *filesize;
		} vals;
		
		struct BArgs::Option my_options[] = {
			{"file", &vals.filename},
			{"size", &vals.filesize},
			{NULL, NULL}
		};
		
		char **pos = BArgs::parse(argc, argv, my_options);
		
		CHECK_EQUAL(vals.filename, "-my--file=.txt=");
		CHECK(strcmp(vals.filename, "5M") != 0);
		CHECK(strcmp(pos[0], "pos1") == 0);
		CHECK(strcmp(pos[1], "-unknown") == 0);
	}
}
