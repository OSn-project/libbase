/* Compile: clang++ -o args1 args1.cpp ../../src/libbase.a -isystem ../../include/ */

#include <stdlib.h>
#include <stdio.h>

#include <base/args.h>

char *filename = NULL;
bool  strict   = false;

struct BArgs::Option options[] = {
	{"output"  , 'o', &filename},
	//{"strict",   0, &strict},
	{NULL, 0, NULL}
};

int main(int argc, char *argv[])
{
	char **positional = BArgs::parse(argc, argv, options);
	
	/* Check for compulsory arguments */
	if (filename == NULL)
	{
		printf("Error: output filename not specified.\n");
		return 1;
	}
	
	/* Print the arguments */
	printf("Inputs:\n");
	for (char **current = positional; *current != NULL; current++)
	{
		printf("\t%s\n", *current);
	}
	
	printf("Output file: %s\n", filename);
	
	free(positional);
	return 0;
}
