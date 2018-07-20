#include <stdlib.h>
#include <base/tuple.h>

uint32 BTuple::length(void **tuple)
{
	if (tuple == NULL) return 0;
	uint32 length = 0;
	
	for (void **current = tuple; *current != NULL; current++)
	{
		length++;
	}
	
	return length;
}
