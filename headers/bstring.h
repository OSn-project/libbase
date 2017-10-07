#ifndef __BSTRING_H__
#define __BSTRING_H__

#include <string.h>
#include "bdefs.h"

class BString
{
	char  *string;
	
	size_t m_size;		// in bytes, excluding the null-terminator. The number will equal the length in characters as long as the string is ASCII. If the string is UTF8, the length can be obtained by calling ->length_utf8().
	
	uint32 __reserved1[4];
	
public:
	BString();
	BString(const char*);
	
	~BString();
	
	bool set(const char *str);	// Returns false if allocating memory for the new string failed.
	
	int32 length();
	int32 length_utf8();
	
	size_t utf8_size(int32 from, int32 to);		// Returns the size of the string between the two indexes in bytes (minus the null-terminator). The character at the end index is excluded (..gth(3, 5) would count characters 3 and 4)

	BString *uppercase();
	BString *lowercase();
	
	const char *c_str();		// Returns a pointer to the internal buffer. It is recommended that you strdup() this.
};

#endif

/*
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
