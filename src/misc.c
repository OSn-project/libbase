#include <string.h>
#include <stdbool.h>

bool streq(const char *a, const char *b)
{
	if (a == NULL || b == NULL)
		return false;

	for (; *a != '\0' || *b != '\0'; a++, b++)
	{
		if (*a != *b)
			return false;
	}

	return true;
}

bool strneq(const char *a, const char *b, size_t max)
{
	if (a == NULL || b == NULL)
		return false;

	for (size_t i = 0; (*a != '\0' || *b != '\0') && i < max; a++, b++, i++)
	{
		if (*a != *b)
			return false;
	}

	return true;
}
