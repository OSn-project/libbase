#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include <UnitTest++/UnitTest++.h>
#include "privablic.h"

#include <base/command.h>

SUITE(BCommand)
{
	TEST (subcmds)
	{
		BCommand commit;

		BCommand *git_subcmds[] = {&commit, NULL};
		BOption git_options[] = {
			{"version", NULL, 0},
			{NULL},
		};
		BCommand git = {"git", NULL, git_subcmds, git_options};

		BOption commit_options[] = {
				{"comment", "m", .value="No comment was given."},
				{"all", "a"},
				{NULL},
		};
		commit = {"commit", NULL, NULL, commit_options};
	}
}
