#include <stdio.h>
#include <base/command.h>
#include <base/misc.h>

int BCommand :: execute(BCommand *cmddef, int argc, char *argv[])
{
	/* First check whether it's a subcommand */
	for (BCommand **subcmd = cmddef->subcmds; *subcmd != NULL; subcmd++)
	{
		if (streq((*subcmd)->name, argv[1]))
			return BCommand::execute(*subcmd, argc - 1, argv + 1);
	}

	/* Else parse the arguments */
}

bool BCommand :: parse (int argc, char *argv[])
{
	for (int i = 1; i < argc; i++)
	{

	}
}

/* Default error handler functions */

static void default_bad_opt(const char *opt)
{
	fprintf(stderr, "No such option: %s\n", opt);
}

static void default_bad_cmd(const char *cmd)
{
	fprintf(stderr, "No such command: %s\n", cmd);
}

/* Set the pointers to them */
void (*BCommand::global_bad_opt)(const char *opt) = default_bad_opt;
void (*BCommand::global_bad_cmd)(const char *cmd) = default_bad_cmd;
