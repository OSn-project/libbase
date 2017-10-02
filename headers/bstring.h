#ifndef __BSTRING_H__
#define __BSTRING_H__

#include <string.h>
#include "bdefs.h"

class BString
{
	char   *string;

public:
	BString();
	BString(const char*);
	
	~BString();
	
	bool set(const char *str);		//! Returns false if allocating memory for the new string failed.
	
	int32 length();
	int32 length_utf8();
	
	const char *c_str();		//! Returns a pointer to the internal buffer. It is recommended that you strdup() this.
};

#endif

/*
 * int32 utf8_size(int32 from, int32 to);
 * BString *uppercase();
 * BString *lowercase();
 * void append(BString *str);
 * void prepend(BString *str);
 * void insert(BString *str);
 * BSList<BString> *split(const char *split_chars);
 * int32 count(char character);
 * int32 count_chars(const char *characters);
 * BString *slice(uint32 start, uint32 end);
 * BString *reverse();
 * void remove(char character);
 * void remove_chars(const char *characters);
 * void strip();
 * char char_at(int32 index);
 */
