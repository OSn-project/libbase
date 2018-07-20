#ifndef __BASE_CMDAPP_H__
#define __BASE_CMDAPP_H__

#include <stdio.h>
#include <base/string.h>
#include <osndef.h>

struct BAppCommand;

class BCmdApp
{
	BAppCommand *commands;		// An array of structs containing information about each command

protected:
	const char *greeting;
	const char *quit_command;
	
	BString  prompt;

public:
	BCmdApp(BAppCommand *commands);
	~BCmdApp();

	void run(int argc, char *argv[]);	// $ myapp list ~/myfile.xyz
	void interactive(FILE *script = NULL);					// $ myapp
										// > list ~/myfile.xyz
};

struct BAppCommand
{
	const char *name;
	void (*function)(BString **positional, void *named);
};

#endif
