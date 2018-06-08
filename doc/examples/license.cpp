/* Compile: clang++ -o cmdapp1 license.cpp ../../src/libbase.a -isystem ../../include/ */

#include <stdio.h>

#include <base/string.h>
#include <base/cmdapp.h>

void help(BString **positional, void *named);

static BAppCommand commands[] = {
	{"help", help},
	{NULL, NULL}
};

int main(int argc, char *argv[])
{
	BCmdApp *app = new BCmdApp(commands);
	app->interactive();
	delete app;
	
	return 0;
}

void help(BString **positional, void *named)
{
	printf("Dial 999 for emergency services.\n");
}
