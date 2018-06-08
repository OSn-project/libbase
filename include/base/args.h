#ifndef __BASE_ARGS_H__
#define __BASE_ARGS_H__

#include <base/string.h>
#include <osndef.h>

class BArgs
{
public:
	struct Option
	{
		const char *name;
		const char  abbr;
		
		char **target;
	};
	
	static void parse(int argc, char *argv[], Option *options);
	
private:
	static Option *find_by_name(char *name, size_t length, Option *options);
	static Option *find_by_abbr(char abbr, Option *options);
};

#endif
