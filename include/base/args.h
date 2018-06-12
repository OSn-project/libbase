#ifndef __BASE_ARGS_H__
#define __BASE_ARGS_H__

#include <base/string.h>
#include <osndef.h>

class BArgs
{
public:
	enum OptionType
	{
		BARG_STRING,
		BARG_BOOL,
		BARG_INT32
	};
	
	struct Option
	{
		const char *name;
		const char  abbr;
		
		//OptionType opt_type;
		
		void *target;
	};
	
	/* This function parses the command-line arguments	*
	 * using the options given.
	 * Any positional arguments are pointed to in the	*
	 * NULL-terminated array that is returned by the	*
	 * function. This function just works with pointers	*
	 * to strings in `argv` and does not duplicate		*
	 * anything.										*/
	
	static char **parse(int argc, char *argv[], Option *options);
	
private:
	static Option *find_by_name(char *name, size_t length, Option *options);
	static Option *find_by_abbr(char abbr, Option *options);
};

#endif
