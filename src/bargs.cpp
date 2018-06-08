#include <string.h>

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
#include<stdio.h>
void BArgs :: parse(int argc, char *argv[], Option *options)
{
	char *arg;
	
	for (int32 i = 1; i < argc; i++)
	{
		arg = argv[i];
		
		if (strncmp(arg, "--", 2) == 0)
		{
			/* In the case of a long argument */
			char *name = arg + 2;
			Option *option = BArgs::find_by_name(name, strchrnul(name, '=') - name, options);
			
			if (option == NULL) goto not_found;
			
			char *value = strchr(arg, '=') + 1;
			
			if (value != NULL)	// If they've just specified the argument without it's value, we can't set anything.
			{
				*option->target = value;
			}
		}
		else if (arg[0] == '-')
		{
			/* In the case of a short argument */
			Option *option = BArgs::find_by_abbr(arg[1], options);
			
			if (option == NULL) goto not_found;
			
			*option->target = argv[++i];	// The next argument will be the value
			
		}
		else
		{
not_found:
			/* In the case of a positional argument */
			printf("Positional: %s\n", arg);
			continue;
		}
	}
}
