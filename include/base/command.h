#ifndef BASE_COMMAND_H_
#define BASE_COMMAND_H_

#include <osndef.h>

#define ARG_NONE	0
#define ARG_STRING	1

struct BOption
{
	const char *name;
	const char *alt_name;

	const char *value;	// Set at parse time, but can be set manually to specify the default value.

	uint8 args_length();
};

struct BCommand
{
	const char *name;
	int (*callback)(const char **positional);

	BCommand **subcmds;
	BOption   *options;		// The array is terminated by a struct where `name` is NULL.

	/* Misc callbacks */
	void (*bad_opt)(const char *opt);	// Overrides the default 'No such option' message.
	void (*bad_cmd)(const char *cmd);

	/* Static functions */
	static int execute(BCommand *cmdspec, int argc, char *argv[]);

private:
	bool parse(int argc, char *argv[]);

public:
	static void (*global_bad_opt)(const char *opt);		// Called in cases where a command doesn't provide its own bad option handler.
	static void (*global_bad_cmd)(const char *cmd);
};

#endif
