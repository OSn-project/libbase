#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "bdefs.h"
#include "utf8.h"
#include "bstring.h"

BString :: BString ()
{
	this->string = NULL;
	this->m_size = 0;
}

BString :: BString (const char *c_str, bool utf8, int32 limit)
{
	if (limit == -1)
	{
		this->string = strdup(c_str);
		this->utf8 = utf8;
			
		this->m_size = strlen(this->string);		// Strlen will return a negative value if the string is longer than INT32_MAX but this is unlikely
	}
	else
	{
		this->string = strndup(c_str, limit);
		this->utf8 = utf8;
			
		this->m_size = (size_t) limit;
	}
}

BString :: BString(const BString& str)
{
	this->string = strndup(str.string, str.m_size);		// We use strNdup just to be safe
	this->m_size = str.m_size;
	this->utf8   = str.utf8;
}

BString :: ~BString ()
{
	free(this->string);
}

bool BString :: set(const char *str)
{
	if (str == NULL) str = "";		// Reset the string if we're given NULL

	/* Change the size of the buffer to	*
	 * match the size of the new string	*/
	
	this->m_size = strlen(str);
	this->string = (char *) realloc(this->string, this->m_size + 1);
	
	if (! this->string)
	{
		/* In case of an error from realloc */
		this->m_size = 0;
		return false;
	}
	
	/* Copy the string */
	strcpy(this->string, str);
	
	return true;
}

int32 BString :: length()
{
	if (this->string == NULL) return 0;
	
	if (this->utf8)
	{
		return utf8_length(this->string);
	}
	else
	{
		return this->m_size;
	}
}

size_t BString :: utf8_size(int32 from, int32 to)
{
	if (from < 0) from += this->length();
	if (to   < 0) to   += this->length();
	if (from < 0 || to < 0) return 0;
	if (from > to) return 0;
	
	size_t size = 0;
	
	for (char *chr = utf8_char_at(this->string, from); chr < utf8_char_at(this->string, to); chr++)
	{
		size++;
	}
	
	return size;
}

BString **BString :: split(char delim)
{
	/* First we need to get the number of strings that		*
	 * the original string will be split into. This is  	*
	 * just the count of delimiters in the string plus 1.	*/
	
	uint32 out_strings = 1 + this->count(delim);
	
	/* Allocate the tuple */
	BString **out = (BString **) calloc(out_strings + 1, sizeof(BString *));		// +1 because it is a null-terminated array.
	
	BString **str_ptr = out;											// This points to the current pointer in the tuple that we're creating. We increment it every time the loop iterates.
	
	for (const char *c = this->string; *c != '\0'; c = (*c == delim) ? c + 1 : (1 + BString::next(delim, c)))	// 'c + 1' if c is a delimiter so that we keep moving and don't get stuck. If it wasn't for the +1 in '1 + BStr..', so that we skip to the character AFTER the next delimiter.
	{
		*str_ptr = new BString(c, this->utf8, this->offset_of(delim, c));
		str_ptr++;
	}
	
	return out;
}

void BString :: tuple_free(BString **tuple)
{
	if (tuple == NULL) return;
	
	for (BString **str = tuple; *str != NULL; str++)
	{
		delete *str;
	}
	
	free(tuple);
}

bool BString :: append(char *str, size_t str_size)
{
	this->string = (char *) realloc(this->string, (this->m_size + str_size + 1) * sizeof(char));	// Remember that the sizes don't include the null-terminator to make things easier. That's why we +1 at the end.
	
	if (! this->string)
	{
		this->m_size = 0;
		return false;
	}
	
	memcpy(this->string + this->m_size, str, str_size + 1);		// +1 to copy the null-terminator too.
	
	this->m_size = this->m_size + str_size + 1;
	
	return true;
}

bool BString :: prepend(char *str, size_t str_size)		// Really isn't too different from append except that we memmove the original string first.
{
	this->string = (char *) realloc(this->string, (this->m_size + str_size + 1) * sizeof(char));
	
	if (! this->string)		// If realloc returns NULL (we have run out of memory), set our size to 0 and return false to signify error.
	{
		this->m_size = 0;
		return false;
	}
	
	memmove(this->string + str_size, this->string, this->m_size + 1);		// +1 to copy the null-terminator to the end too.
	memcpy(this->string, str, str_size);
	
	this->m_size = this->m_size + str_size + 1;
	
	return true;
}

bool BString :: insert(char *str, size_t str_size, int32 offset)
{
	if (offset < -(this->length()) || offset > this->length()) return false;
	if (offset < 0) offset += this->length();	// Get rid of negative lengths
	
	this->string = (char *) realloc(this->string, (this->m_size + str_size + 1) * sizeof(char));

	if (! this->string)
	{
		this->m_size = 0;
		return false;
	}
	
	char *insert_pos = this->utf8 ? utf8_char_at(this->string, offset) : &this->string[offset];		// Get a pointer to where the string will be inserted.
	
	memmove(insert_pos + str_size, insert_pos, this->m_size - (insert_pos - this->string)); 		// We use memmove instead of memcpy because the areas may overlap.
	memcpy(insert_pos, str, str_size);
	
	this->m_size = this->m_size + str_size + 1;
	
	return true;
}

BString *BString :: uppercase()
{
	char *upper_str = strdup(this->string);
	
	for (char *chr = upper_str; *chr != '\0'; chr++)
	{
		*chr = toupper(*chr);
	}
	
	BString *upper = new BString(upper_str);
	
	free(upper_str);	
	return upper;
}

BString *BString :: lowercase()
{
	char *lower_str = strdup(this->string);
	
	for (char *chr = lower_str; *chr != '\0'; chr++)
	{
		*chr = tolower(*chr);
	}
	
	BString *lower = new BString(lower_str);
	
	free(lower_str);	
	return lower;
}

bool BString :: equals(char *str, size_t str_size)
{
	if (str_size == 0 && this->m_size != 0) return false;
	
	return (strncmp(this->string, str, str_size) == 0);
}

void BString :: remove(int32 start, int32 length)
{
	if (length <= 0) return;
	if (start  <  0) start += this->length();
	if (start  <  0) return;					// If start is out of range even after adding the string's length...
	if (length > this->length() - start) return;	// Check that the length they want to remove isn't longer than the rest of the string.
	
	this->m_size -= this->utf8_size(start, length);

	memmove((void *) this->char_at(start), (void *) this->char_at(start + length), this->utf8_size(start + length, this->length()) + 1);	// +1 to copy the null-term too.
	this->string = (char *) realloc(this->string, this->m_size);		// Shrink the memory because we don't need as much anymore.
}

int32 BString :: count(char chr)
{
	if (this->string == NULL) return 0;
	int32 count = 0;
	
	if (this->utf8)
	{
		for (char *c = this->string; *c != '\0'; c = utf8_nextchar(c))
		{
			if (*c == chr) count++;
		}
	}
	else
	{
		for (char *c = this->string; *c != '\0'; c++)
		{
			if (*c == chr) count++;
		}
	}
	
	return count;
}

int32 BString :: count_chars(const char *chars)
{
	if (this->string == NULL) return 0;
	int32 count = 0;
	
	if (this->utf8)
	{
		for (char *str_char = this->string; *str_char != '\0'; str_char = utf8_nextchar(str_char))
		{
			for (char *chr = (char *) chars; *chr != '\0'; chr = utf8_nextchar(chr))
			{
				if (utf8_charcmp(str_char, chr)) count++;
			}
		}
	}
	else
	{
		for (char *str_char = this->string; *str_char != '\0'; str_char++)		// For each character in the string
		{
			for (char *chr = (char *) chars; *chr != '\0'; chr++)		// We loop over each of the characters to count
			{
				if (*str_char == *chr) count++;						// and compare the two.
			}
		}
	}
	
	return count;
}

int32 BString :: index_of(char chr, const char *start)		// The following three functions are duplicates of this one only with slight alterations.
{
	/* Take care of the start pointer */
	if (! start)
	{
		start = this->string;
	}
	else
	{
		/* Check that the pointer is within our string */
		
		if (! (this->string <= start && start < this->string + this->m_size)) return -1;
	}
	
	/* Get the index of the given character */
	int32 index = 0;
	
	for (char *c = (char *) start; *c != chr; c = (this->utf8 ? utf8_nextchar(c) : c + 1))		// const grrrhh
	{
		if (*c == '\0')
		{
			return -1;
		}
		
		index++;
	}
	
	return index;
}

int32 BString :: index_of_utf8(const char *chr, const char *start)
{
	/* Take care of the start pointer */
	if (! start)
	{
		start = this->string;
	}
	else
	{
		/* Check that the pointer is within our string */
		
		if (! (this->string <= start && start < this->string + this->m_size)) return -1;
	}
	
	/* Get the index of the given character */
	int32 index = 0;
	
	for (char *c = (char *) start; utf8_charcmp(c, chr) != true; c = (this->utf8 ? utf8_nextchar(c) : c + 1))
	{
		if (*c == '\0')
		{
			return -1;
		}
		
		index++;
	}
	
	return index;
}

int32 BString :: offset_of(char chr, const char *start)
{
	/* Take care of the start pointer */
	if (! start)
	{
		start = this->string;
	}
	else
	{
		/* Check that the pointer is within our string */
		
		if (! (this->string <= start && start < this->string + this->m_size)) return -1;
	}
	
	/* Get the offset of the given character */
	int32 offset = 0;
	
	for (char *c = (char *) start; *c != chr; c++)
	{
		if (*c == '\0')
		{
			return -1;
		}
		
		offset++;
	}
	
	return offset;
}

int32 BString :: offset_of_utf8(const char *chr, const char *start)
{
	/* Take care of the start pointer */
	if (! start)
	{
		start = this->string;
	}
	else
	{
		/* Check that the pointer is within our string */
		
		if (! (this->string <= start && start < this->string + this->m_size)) return -1;
	}
	
	/* Get the offset of the given character */
	int32 offset = 0;
	
	for (char *c = (char *) start; utf8_charcmp(c, chr) != true; c++)
	{
		if (*c == '\0')
		{
			return -1;
		}
		
		offset++;
	}
	
	return offset;
}

const char *BString :: next(char chr, const char *start)
{
	const char *c;

	for (c = start; *c != '\0'; c++)
	{
		if (*c == chr) return c;
	}
	
	return c;
}

const char *BString :: next(char chr, BString *start)
{
	return BString::next(chr, start->c_str());
}

const char *BString :: char_at(int32 index)
{
	if (index < 0) index += this->length();
	if (index > this->length() || index < 0) return "";
	
	if (this->utf8)
	{
		return utf8_char_at(this->string, index);
	}
	else
	{
		return &this->string[index];
	}
}
