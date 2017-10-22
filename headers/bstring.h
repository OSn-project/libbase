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
	
	bool        append(char *str, size_t str_size);		// str_size = the size in bytes minus the null-terminator (like what strlen returns you)
	inline bool append(char *str);
	inline bool append(BString *str);
	bool        prepend(char *str, size_t str_size);
	inline bool prepend(char *str);
	inline bool prepend(BString *str);
	bool        insert(BString *str, int32 offset);
	
	BString *uppercase();
	BString *lowercase();
	
	inline const char *char_at(int32 index);
	inline const char *char_at_utf8(int32 index);
	
	inline const char *c_str();		// Returns a pointer to the internal buffer. It is recommended that you strdup() this.
};

/* Inlines */

inline bool BString :: append(char *str)
{
	return this->append(str, strlen(str));
}

inline bool BString :: append(BString *str)
{
	return this->append(str->string, str->m_size);
}

inline bool BString :: prepend(char *str)
{
	return this->prepend(str, strlen(str));
}

inline bool BString :: prepend(BString *str)
{
	return this->prepend(str->string, str->m_size);
}

/*inline const char *char_at(int32 index)
{
	if (index < -(this->m_size) || index > )
	
	if (index < 0)
	{
		index += this->m_size;
	}

}*/

inline const char *char_at_utf8(int32 index)
{
	if (index < -(length_utf8(this->string)) || index > utf8_length(this->string) - 1) return NULL;
	
	if (index < 0)
	{
		index += this->length_utf8();	// Adding a string's length to to index transforms it into its positive equivalent.
	}
	
	return utf8_char_at(this->string, index);
}

inline const char *BString :: c_str ()
{
	return (const char *) (this->string ? this->string : "");
}

#endif

/*
 * BSList<BString> *split(const char *split_chars);
 * int32 count(char character);
 * int32 count_chars(const char *characters);
 * BString *slice(uint32 start, uint32 end);
 * BString *reverse();
 * void remove(char character);
 * void remove_chars(const char *characters);
 * void strip();
 * bool starts_with(BString *str);
 * bool ends_with(BString *str);
 * char char_at(int32 index);
 */
