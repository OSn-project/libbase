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

BString :: BString (const char *c_str, bool utf8)
{
	this->string = strdup(c_str);
	this->utf8 = utf8;
		
	this->m_size = strlen(this->string);		// Strlen will return a negative value if the string is longer than INT32_MAX but this is unlikley
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
	if (from < 0 || to < 0) return 0;
	if (from > to) return 0;
	
	size_t size = 0;
	
	for (char *chr = utf8_char_at(this->string, from); chr < utf8_char_at(this->string, to); chr++)
	{
		size++;
	}
	
	return size;
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
