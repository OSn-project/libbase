#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/base/misc.h"
#include "../include/base/args.h"

BArgs::Option *BArgs :: find_by_name(char *name, size_t length, Option *options)
{
	for (Option *option = options; option->name != NULL; option++)
	{
		if (strneq(option->name, name, length))
		{
			return option;
		}
	}
	
	return NULL;
}

char **BArgs :: parse(int argc, char *argv[], Option *options)
{
	char **positional =  (char **) calloc(argc + 1, sizeof(char *));	// FIXME: There may well be excess room since it not all arguments may be positionals.
	uint32 pos_count = 0;	// The number of positional arguments so far.
	
	for (int32 index = 1; index < argc;)
	{
		char *arg = argv[index];
		
		if (arg[0] == '-')
		{
			/* In the case of a named argument */
			char *name = arg + 1;
			Option *option = BArgs::find_by_name(name, strchrnul(name, '=') - name, options);
			
			if (option == NULL)
			{
				/* If the given option is unknown, complain and add it as a positional argument. */
				
				fprintf(stderr, "%s: Unexpected option '%s'\n", argv[0], arg);
				goto add_as_positional;
			}
			
			if (index == argc - 1)
			{
				fprintf(stderr, "%s: Value not specified for option '%s'\n", argv[0], arg);
				goto add_as_positional;
			}
			
			*((char **) option->target) = argv[index + 1];
			
			index += 2;
		}
		else
		{
		add_as_positional:
			/* In the case of a positional argument */
			positional[pos_count] = arg;				// Add the pointer to the list of positionals.
			pos_count++;
			
			index += 1;
		}
		
		continue;
	}
	
	return positional;
}
