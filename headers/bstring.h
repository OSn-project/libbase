#ifndef __BSTRING_H__
#define __BSTRING_H__

#include <string.h>
#include "bdefs.h"

class BString
{
	char   *string;
	size_t  allocated_size;

public:
	BString();
	BString(const char*);
	
	~BString();
	
	bool set(const char *str);		//! Returns false if allocating memory for the new string failed.
	
	int32 length();
	int32 length_utf8();
	
	const char *c_str();		//! Returns a pointer to the internal buffer. It is recommended that you strdup() this.

private:
	
	/* UTF-8 functions */
	char *__utf8_nextchar(char *start);
	char *__utf8_char_at(int32 index);
	void  __utf8_first(char *dest, int32 chars);
	void  __utf8_rest (char *dest, int32 start);
	void  __utf8_slice(char *dest, int32 start, int32 chars);
	int32 __utf8_length(char *start = NULL);		// <- If no pointer is given, this->string is implied.
};

#endif

/*
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
