#ifndef ENDIAN_INTS_H_
#define ENDIAN_INTS_H_

#include <endian.h>
#include "defs.h"

struct uint16le
{
private:
	uint16 value;

public:
	operator uint16() const
	{
		return le16toh(this->value);
	}

	uint16le operator = (const uint16 rhs)
	{
		this->value = htole32(rhs);
	}
};

#endif
