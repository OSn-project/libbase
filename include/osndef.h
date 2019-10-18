#ifndef OSNDEF_H_
#define OSNDEF_H_

#include <stddef.h>
#include "base/defs.h"

/* These are only to be used when you	*
 * explicitly do not want the value to	*
 * be treated as an integer.			*/
#ifndef __cplusplus
typedef uint32 dword;
typedef uint64 qword;
#else
union dword
{
	uint32 value;
	uint8  bytes[4];

	inline dword () {}
	inline dword (const uint32& val) { this->value = val; }
	inline operator uint32() const { return this->value; }
};

union qword
{
	uint64 value;
	uint8  bytes[8];

	inline qword () {}
	inline qword (const uint64& val) { this->value = val; }
	inline operator uint64() const { return this->value; }
};
#endif

#define owning	// This modifier is used when a function returns a pointer to malloced memory that it is the user's responsibility to free().

#endif
