/* Wrapper for a null-terminated array of pointers. */

#ifndef __BTUPLE_H__
#define __BTUPLE_H__

#include <osndef.h>

class BTuple
{
public:
	static uint32 length(void **tuple);
};

#endif
