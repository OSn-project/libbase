/* Compile: clang++ -o args1 args1.cpp ../../src/libbase.a -isystem ../../include/ */

#include <stdio.h>
#include <base/args.h>

char *filename = NULL;
bool strict;

struct BArgs::Option options[] = {
	{"file"  , 'f', &filename},
	//{"strict",   0, &strict},
	{NULL, 0, NULL}
};

int main(int argc, char *argv[])
{
	BArgs::parse(argc, argv, options);
	
	printf("File: %s\n", filename);
}
