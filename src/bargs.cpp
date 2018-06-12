#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../include/base/misc.h"
#include "../include/base/args.h"

BArgs::Option *BArgs :: find_by_name(char *name, size_t length, Option *options)
{
	for (Option *option = options; option->target != NULL; option++)
	{
		if (strneq(option->name, name, length))
		{
			return option;
		}
	}
	
	return NULL;
}

BArgs::Option *BArgs :: find_by_abbr(char abbr, Option *options)
{
	for (Option *option = options; option->target != NULL; option++)
	{
		if (option->abbr == abbr)
		{
			return option;
		}
	}
	
	return NULL;
}

char **BArgs :: parse(int argc, char *argv[], Option *options)
{
	char **positional =  (char **) calloc(argc + 1, sizeof(char *));	// FIXME: There may well be excess room since it not all arguments may be positionals.
	uint32 pos_count = 0;
	
	for (int32 i = 1; i < argc; i++)
	{
		char *arg = argv[i];
		
		if (strneq(arg, "--", 2))
		{
			/* In the case of a long argument */
			char *name = arg + 2;
			Option *option = BArgs::find_by_name(name, strchrnul(name, '=') - name, options);
			
			if (option == NULL) goto not_found;
			
			char *value = strchr(arg, '=') + 1;
			
			if (value != NULL)	// If they've just specified the argument without its value, we can't set anything.
			{
				*option->target = value;
			}
		}
		else if (strneq(arg, "-", 1))
		{
			/* In the case of a short argument */
			Option *option = BArgs::find_by_abbr(arg[1], options);
			
			if (option == NULL) goto not_found;
			
			{
				if (strlen(arg) == 2)
				{
					/* If the argument is the short name alone,		*
					 * assume the next argument to be the value:	*
					 * ./prog -f /tmp/file							*/
					
					*option->target = argv[++i];
				}
				else if (arg[2] == '=')
				{
					/* If there's an '=' after the short name:		*
					 * ./prog -f=/tmp/file							*/
					
					*option->target = arg + 3;
				}
				else
				{
					/* Complain if they've tried to give us the value		*
					 * straight after the short name (like getopt allows):	*
					 * ./prog -f/tmp/file									*/
					
					fprintf(stderr, "%s: Please split option name and value with '=':\t-%c=%s\n", argv[0], arg[1], arg + 2);
				}
			}
			
		}
		else
		{
			/* In the case of a positional argument */
			positional[pos_count] = arg;				// Add the pointer to the list of positionals.
			pos_count++;
		}
		
		continue;
		
	not_found:
		fprintf(stderr, "%s: Unexpected argument '%s'\n", argv[0], arg);
	}
	
	return positional;
}
