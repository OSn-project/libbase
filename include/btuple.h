/* Wrapper for a null-terminated array of pointers.		*
 * I don't really know whether this is even necessary.	*/

#ifndef __BTUPLE_H__
#define __BTUPLE_H__

#include "bdefs.h"

template <typename *T> class BTuple
{
	T **values;
	
	size_t length();
}

template <typename *T> size_t BTuple :: length()
{
	size_t length = 0;
	
	for (T **current = this->values; *current != NULL; current++)
	{
		length++;
	}
	
	return length;
}

#endif
