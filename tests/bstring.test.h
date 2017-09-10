#ifndef __BSTRING_MOCK_H__
#define __BSTRING_MOCK_H__

#include "bdefs.h"
#include "bstring.h"

class MockBString : public BString {
public:
	MOCK_METHOD0(length, int32());
	MOCK_METHOD0(length_utf8, int32());
	MOCK_CONST_METHOD0(c_str, char *());
};

#endif
