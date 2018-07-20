/* BString executes different code depending on whether		*
 * the string contains UTF-8 because the non UTF-8 aware 	*
 * code is more efficient. Note: Some BString methods have	*
 * static counterparts which operate on C-strings.			*/

#ifndef __BSTRING_H__
#define __BSTRING_H__

#include <stdio.h>
#include <string.h>
#include <osndef.h>

class BString
{
protected:
	char  *string;
	size_t m_size;		// in bytes, excluding the null-terminator. The number will equal the length in characters as long as the string is ASCII. If the string is UTF8, the length can be obtained by calling ->length() with ->utf8 set to true.
	
public:
	bool utf8;
	
	BString();
	BString(const char* str, bool utf8 = false, int32 limit = -1);		// Limit *would* be size_t if it wasn't to be an optional argument which we had to know whether it was unset
	BString(const BString& str);
	
	~BString();
	
	bool set(const char *str);		// Returns false if allocating memory for the new string failed.
	bool set(const BString *str);
	void clear();
	
	int32 length();			// This isn't size_t because otherwise we couldn't do things like -(str->length()) without casting.
	
	size_t size();								// Returns the size of the string in bytes, excluding the null-terminator. Not guaranteed to be the string's length in characters.
	size_t utf8_size(int32 from, int32 to);		// Returns the size of the string between the two indexes in bytes. The character at the end index is excluded (..ize(3, 5) would count characters 3 and 4)

	BString **split(char delim);				// Splits the string with the given delimiter and returns a tuple (a null-terminated array of pointers) of new BStrings containing the split string.
	static void tuple_free(BString **tuple);	// A helper function to free tuples returned by ->split()
	
	bool        append(char *str, size_t str_size);		// str_size = the size in bytes minus the null-terminator (like what strlen returns you)
	inline bool append(char *str);
	inline bool append(BString *str);
	bool        prepend(char *str, size_t str_size);	// NOTE: THE ADDRESS RETURNED BY ::c_str() MAY CHANGE with calls to ::append(), ::prepend(), ::insert() and ::remove() because they call realloc on the buffer.
	inline bool prepend(char *str);
	inline bool prepend(BString *str);
	bool        insert(char *str, size_t str_size, int32 offset);
	inline bool insert(char *str, int32 offset);
	inline bool insert(BString *str, int32 offset);
	void        append_c(char c);
	
	BString *uppercase();
	BString *lowercase();
	
	static bool equals(BString *a, const char *b, size_t size);
	static bool equals(BString *a, const char *b);
	static bool equals(BString *a, BString *b);
	inline bool equals(const char *b, size_t size);
	inline bool equals(const char *b);
	inline bool equals(BString *b);
	
	void remove(int32 start, int32 length);				// length is in chars
	void remove(const char *start, const char *end);
	void remove_char(char chr);							// Removes all occurences of the given character
	//void remove_chars(const char *characters);		// For multiple characters, or UTF-8 characters.
	
	int32 count(char chr);					// If you want to count a UTF-8 character then you need to use ->count_chars().
	int32 count_chars(const char *chr);
	int32 index_of (char chr, const char *start = NULL);				// Returns the index of the given character or -1 if not found. 
	int32 index_of_utf8 (const char *chr, const char *start = NULL);
	int32 offset_of(char chr, const char *start = NULL);				// Like ->index_of() but returns offset in bytes instead characters.
	int32 offset_of_utf8(const char *chr, const char *start = NULL);
	
	const char        *first(char chr);							// Returns a pointer to the first occurence of the given character, or NULL if not found.
	static const char *next (char chr, const char *start);		// Returns a pointer to the next occurence of the given character in the string, or to the null terminator if nonexistent.
	//int32 next(char chr, int32 start)
	
	int32 offset_of_ptr(const char *str);	// Returns the index in characters of the given pointer in the string. Returns -1 if it points outside of the string.
	const char *char_at(int32 index);
	
	inline const char *c_str();		// Returns a pointer to the internal buffer. It is recommended that you strdup() this.

	/* File access */
	static BString *read_file(const char *path);
	static BString *read_file(FILE *file, int32 bytes = -1, size_t chunk_size = 256);	// Creates a BString from the next n bytes in the given file. Stops at null-bytes.
	bool            write_file(const char *path);		// These return true on success and false on failure
	bool            write_file(FILE *file);

	/* Static helper functions */
	static void read_line(FILE *file, BString *str);	// Read a line of text into a string. If the string already contains text, then the text is appended. The trailing '\n' character is stripped.

private:
};

#include "string_inlines.h"

#endif

/*
 * void remove_between(int32 start, int32 end);
 * void remove_between(char *start, char *end);
 * BPList<BString> *split(const char *split_chars);
 * BString *slice(uint32 start, uint32 end);
 * int32 index_of_str(char *str, size_t size);
 * BString *reverse();
 * void delete(int32 start, int32 length);
 * void remove(char character);
 * void remove_chars(const char *characters);
 * void strip();
 * bool starts_with(BString *str);
 * bool ends_with(BString *str);
 * bool serialize8(FILE *file);
 * static BString *unserialize8(FILE *file);
 * 
 */
