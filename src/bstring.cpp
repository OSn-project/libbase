#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/osndef.h"
#include "../include/utf8.h"
#include <base/string.h>

BString :: BString ()
{
	this->string = strdup("");
	this->m_size = 0;
	this->utf8   = false;
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

void BString :: clear()
{
	this->set(NULL);
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

size_t BString :: size()
{
	return this->m_size;
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
	
	memmove(insert_pos + str_size, insert_pos, this->m_size - (insert_pos - this->string) + 1); 		// Create space for the inserted string by shifting the text AFTER the insert position by the length of the string. +1 to shift the null terminator too. We use memmove instead of memcpy because the areas may overlap.
	memcpy(insert_pos, str, str_size);			// Copy the string into the space that we newly created.
	
	this->m_size = this->m_size + str_size;
	
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

bool BString :: equals(const char *str, size_t str_size)
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
	
	this->remove(this->char_at(start), this->char_at(start + length));
}

void BString :: remove(const char *start, const char *end)
{
	if (end <= start) return;
	if (! (this->string <= start && start < this->string + this->m_size)) return;
	if (! (this->string <  end   && end   < this->string + this->m_size)) return;
	
	memmove((void *) start, (void *) end, this->string + this->m_size - end + 1);	// +1 to copy the null-term too.

	this->m_size -= end - start;									// Decrease the size to that of the string after the given range has been removed.
	this->string = (char *) realloc(this->string, this->m_size);		// Shrink the memory because we don't need as much anymore.
}

void BString :: remove_char(char chr)
{
	if (chr == '\0') return;
	
	this->m_size -= this->count(chr);
	
	char *new_str = (char *) calloc(this->m_size + 1, sizeof(char));		// +1 for null-term.
	
	for (char *p = this->string, *q = new_str; *p != '\0' && q - new_str < this->m_size; p++)
	{
		if (*p != chr)
		{
			*q = *p;
			q++;
		}
	}
	
	free(this->string);
	this->string = new_str;
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
		
		if (! (this->string <= start && start < (this->string + this->m_size) )) return -1;
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

const char *BString :: first(char chr)
{
	const char *ret = BString::next(chr, this->c_str());
	return (*ret != '\0') ? ret : NULL;
}

int32 BString :: offset_of_ptr(const char *str)
{
	if (str == NULL) return -1;
	if (str == this->string + this->m_size) return this->length();		// If they are pointing to the null-terminator, the correct result should be the string's length. Our loop doesn't account for this though so we have to check for it specifically.
	
	int32 offset = 0;
	
	for (char *c = this->string; *c != '\0'; c = utf8_nextchar(c))
	{
		if (c >= str) return offset;
		
		offset++;
	}
	
	return -1;
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

BString *BString :: load_file(const char *path)
{
	FILE *file   = fopen(path, "r");
	if (! file) return NULL;
	BString *str = BString::load_file(file);
	fclose(file);

	return str;
}

BString *BString :: load_file(FILE *file, int32 bytes, size_t chunk_size)
{
	if (bytes == 0) return new BString();			// If we are to read 0 bytes, we just return an empty string.
	if (file == NULL || bytes < -1) return NULL;	// If they didn't pass us a file pointer, or they want us to read an invalid count of bytes, return NULL.
	if (chunk_size == 0) return NULL;
	
	BString *str = new BString();		// ->string in a new BString is NULL
	
	if (bytes == -1)
	{
		/* The size of the file is indefinite, and so we can't allocate	*
		 * the right size buffer in advance. We therefore reallocate	*
		 * the buffer every 64 bytes that it grows.						*/
		
		str->string = (char *) malloc(chunk_size * sizeof(char));	// Allocate the memory at its initial size
		
		if (! str->string) return NULL;
		
		int32 tmp;
		int32 chunks = 1;
		for (str->m_size = 0;; str->m_size++)
		{
			tmp = fgetc(file);

			if (str->m_size % chunk_size == chunk_size - 1)	// If we're running out of room in our buffer, make it bigger.
			{
				str->string = (char *) realloc(str->string, (chunks + 1) * chunk_size * sizeof(char));
				chunks++;
				
				if (str->string == NULL) return NULL;		// If we ran out of memory return NULL to signify an error.
			}
						
			if (tmp == EOF) break;
			
			str->string[str->m_size] = (char) tmp;
		}
		
		str->string[str->m_size] = '\0';		// Add a null-terminator at the end of the string
	}
	else
	{
		str->string = (char *) malloc(bytes + 1 * sizeof(char));
		fgets(str->string, bytes + 1, file);						// The byte count we are given probably doesn't take the null-terminator into account. We therefore allocate bytes + 1 memory. fgets() reads ONE LESS than the given number bytes (so that there is always room for the null-term), and so we again tell it to read bytes + 1 bytes.

		str->m_size = strnlen(str->string, bytes + 1);			// Even if we did allocate too much memory (ie. if a null byte was reached before the given amount of bytes were read), the redundant memory won't cause any problems and will be freed the next time realloc() is called, since the number of bytes *needed* by the string is stored in ->m_size.
	}
	
	return str;
}

bool BString :: save_file(const char *path)
{
	FILE *file = fopen(path, "w");
	if (file == NULL) return false;
	
	bool rc = this->save_file(file);
	if (rc == false) return false;
	
	fclose(file);
	
	return true;
}

bool BString :: save_file(FILE *file)
{
	if (file == NULL) return false;
	
	int rc = fwrite(this->string, this->m_size * sizeof(char), 1, file);
	
	return (rc == 1);
}

void BString :: read_line(FILE *file, BString *str)
{
	size_t allocated = str->m_size;	// Allocated size
	int32 tmp;
	
	for (char *current = str->string + str->m_size;; current++)
	{	
		tmp = fgetc(file);
		
		if (tmp == '\n' || tmp == EOF) break;
		
		if (str->m_size == allocated)
		{
			str->string = (char *) realloc(str->string, str->m_size + 64);
			allocated += 64;
		}
		
		*current = (char) tmp;
		str->m_size++;
	}
	
	/* Get rid of any excess space */
	str->string = (char *) realloc(str->string, str->m_size + 1);
	str->string[str->m_size] = '\0';	// Add null-terminator
}
