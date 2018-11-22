#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/osndef.h"
#include "../include/utf8.h"
#include "../include/base/misc.h"
#include "../include/base/string.h"

BString :: BString ()
{
	this->string = NULL;
	this->m_size = 0;
	this->utf8   = false;
}

BString :: BString (const char *c_str, bool utf8)
{
	this->string = strdup(c_str);
	this->utf8 = utf8;

	this->m_size = strlen(c_str);		// Strlen will return a negative value if the string is longer than INT32_MAX but this is unlikely
}

BString :: BString (const char *c_str, size_t size, bool utf8)
{
	this->string = (char *) malloc(size + 1);
	memcpy(this->string, c_str, size);
	this->string[size] = '\0';

	this->utf8 = utf8;
	this->m_size = size;
}

BString :: BString(const BString *str)
{
	this->string = (char *) malloc(str->m_size + 1);
	memcpy(this->string, str->string, str->m_size);
	this->string[str->m_size] = '\0';

	this->m_size = str->m_size;
	this->utf8   = str->utf8;
}

BString :: BString(const BString& str)
{
	this->string = (char *) malloc(str.m_size + 1);
	memcpy(this->string, str.string, str.m_size);
	this->string[str.m_size] = '\0';

	this->m_size = str.m_size;
	this->utf8   = str.utf8;
}

BString :: ~BString ()
{
	free(this->string);
}

const char *BString :: set(const char *str)
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
		return NULL;
	}
	
	/* Copy the string */
	strcpy(this->string, str);
	
	return str;
}

const BString *BString :: set(const BString *str)
{
	this->m_size = str->m_size;
	this->string = (char *) malloc(this->m_size);
	
	if (this->string == NULL) goto error;
	
	memcpy(this->string, str->string, this->m_size);
	
	return str;
	
error:
	this->m_size = 0;
	this->string = NULL;
	
	return NULL;
}

void BString :: switch_string(char *str, size_t size)
{
	/* Take ownership of the given malloced string.	*
	 * The size should exclude the null terminator.	*/

	free(this->string);

	this->string = str;
	this->m_size = size;
}

void BString :: clear()
{
	free(this->string);
	this->string = NULL;
	this->m_size = 0;
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

void BString :: append_c(char c)
{
	this->string = (char *) realloc(this->string, this->m_size + 2);	// +1 for new char; +1 for '\0'
	this->string[this->m_size] = c;
	this->string[this->m_size + 1] = '\0';
	this->m_size++;
}

BString *BString :: concat(BString *str1, ...)
{
	va_list ap;
	BString *str = new BString(str1);
	
	/* First add up all of the strings' lengths */
	va_start(ap, str1);
	for (BString *current; (current = va_arg(ap, BString *)) != NULL;)
	{
		str->m_size += current->m_size;
	}
	va_end(ap);
	
	/* Reallocate enough room */
	str->string = (char *) realloc(str->string, str->m_size + 1);
	
	/* Copy the strings in */
	char *end = str->string + str->m_size;

	va_start(ap, str1);
	for (BString *current; (current = va_arg(ap, BString *)) != NULL;)
	{
		memcpy(end, current->string, current->m_size);
		end += current->m_size;
	}
	va_end(ap);
	
	*end = '\0';	// Null-terminator
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

BString *BString :: reverse(BString *out)	// FIXME: UTF-8
{
	char *reversed = (char *) malloc(this->m_size + 1);

	for (char *src = this->string, *dest = reversed + this->m_size - 1; *src != '\0'; src++, dest--)
	{
		*dest = *src;
	}

	out->switch_to(reversed, this->m_size);

	return out;
}

bool BString :: equals(BString *str, const char *c_str, size_t size)
{
	return strneq(str->string, c_str, size);
}

bool BString :: equals(BString *str, const char *c_str)
{
	return streq(str->string, c_str);
}

bool BString :: equals(BString *str_a, BString *str_b)
{
	if (str_a->m_size != str_b->m_size) return false;
	
	return streq(str_a->string, str_b->string);
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

BString *BString :: resize(int32 start, int32 end, BString *out, char fill_char)
{
	uint32 preceding = -b_min(start, 0);
	uint32 lclip     =  b_max(start, 0);
	uint32 rclip     = -b_min(end, 0);
	uint32 following =  b_max(end, 0);

	if (lclip + rclip >= this->m_size)
	{
		out->set("");
	}
	else
	{
		size_t final_size = preceding + (this->m_size - lclip - rclip) + following;

		char *resized = (char *) malloc(final_size + 1);
		char *tmp = resized;

		/* Compose the resulting string */
		memset(tmp, fill_char, preceding);	// Preceding fillers
		tmp += preceding;

		memcpy(tmp, this->string + lclip, this->m_size - lclip - rclip);
		tmp += this->m_size - lclip - rclip;

		memset(tmp, fill_char, following);
		tmp += following;

		*tmp = '\0';

		out->switch_to(resized, final_size);
	}

	return out;
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

const char *BString :: next(char chr, const char *current)
{
	if (current == NULL) current = this->string;
	
	while (*current != chr && current < this->string + this->m_size)	// The second condition checks whether we've gone out of the string's bounds.
	{
		current++;
	}
	
	return current;
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

char *BString :: switch_to(char *str)
{
	return this->switch_to(str, strlen(str));
}
char *BString :: switch_to(char *str, size_t size)
{
	/* Free current string */
	free(this->string);

	this->string = str;
	this->m_size = size;

	return str;
}

BString *BString :: read_file(const char *path)
{
	FILE *file   = fopen(path, "r");
	if (! file) return NULL;
	BString *str = BString::read_file(file);
	fclose(file);

	return str;
}

BString *BString :: read_file(FILE *file, int32 bytes, size_t chunk_size)
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
		
		str->string = (char *) realloc(str->string, chunk_size * sizeof(char));	// Allocate the memory at its initial size
		
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
		str->string = (char *) realloc(str->string, bytes + 1 * sizeof(char));
		fgets(str->string, bytes + 1, file);						// The byte count we are given probably doesn't take the null-terminator into account. We therefore allocate bytes + 1 memory. fgets() reads ONE LESS than the given number bytes (so that there is always room for the null-term), and so we again tell it to read bytes + 1 bytes.

		str->m_size = strnlen(str->string, bytes + 1);			// Even if we did allocate too much memory (ie. if a null byte was reached before the given amount of bytes were read), the redundant memory won't cause any problems and will be freed the next time realloc() is called, since the number of bytes *needed* by the string is stored in ->m_size.
	}
	
	return str;
}

bool BString :: write_file(const char *path)
{
	FILE *file = fopen(path, "w");
	if (file == NULL) return false;
	
	bool rc = this->write_file(file);
	if (rc == false) return false;
	
	fclose(file);
	
	return true;
}

bool BString :: write_file(FILE *file)
{
	if (file == NULL) return false;
	
	int rc = fwrite(this->string, this->m_size * sizeof(char), 1, file);
	
	return (rc == 1);
}

void BString :: read_line(FILE *file, BString *str)
{
	size_t allocated = str->m_size;	// Allocated size
	int32 tmp;
	
	while (true)
	{	
		tmp = fgetc(file);
		
		if (tmp == '\n' || tmp == EOF) break;
		
		if (str->m_size == allocated)
		{
			str->string = (char *) realloc(str->string, str->m_size + 64);
			allocated += 64;
		}
		
		str->string[str->m_size] = (char) tmp;
		str->m_size++;
	}
	
	/* Get rid of any excess space */
	str->string = (char *) realloc(str->string, str->m_size + 1);
	str->string[str->m_size] = '\0';	// Add null-terminator
}

int sprintf(BString *str, const char *format, ...)
{
	va_list ap;
	
	/* Free the current string memory */
	free(str->string);
	
	/* Open the string as a file stream;	*
	 * we need to do this so that the size	*
	 * is not limited.						*/
	FILE *fp = open_memstream(&str->string, &str->m_size);
	
	va_start(ap, format);
	int rc = vfprintf(fp, format, ap);
	va_end(ap);
	
	fflush(fp);
	fclose(fp);
	
	return rc;
}
