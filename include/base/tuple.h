/* Wrapper for a null-terminated array of pointers. */

#ifndef __BTUPLE_H_
#define __BTUPLE_H_

#include <osndef.h>

class BTuple
{
public:
	static uint32 length(void **tuple);
};

#endif
