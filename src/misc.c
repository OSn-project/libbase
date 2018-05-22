#include <string.h>
#include <stdbool.h>

bool streq(const char *a, const char *b)
{
	if (a == NULL || b == NULL)
		return false;
	
	return (strcmp(a, b) == 0);
}
