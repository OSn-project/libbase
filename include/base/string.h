/* BString executes different code depending on whether		*
 * the string contains UTF-8 because the non UTF-8 aware 	*
 * code is more efficient. Note: Some BString methods have	*
 * static counterparts which operate on C-strings.			*/

#ifndef BSTRING_H_
#define BSTRING_H_

#include <osndef.h>
#include <stdio.h>
#include <stdarg.h>

template <typename T> class BArray;

class BString
{
protected:
	char  *string;
	size_t m_size;		// in bytes, excluding the null-terminator. The number will equal the length in characters as long as the string is ASCII. If the string is UTF8, the length can be obtained by calling ->length() with ->utf8 set to true.
	
public:
	BString();
	BString(const char* str);
	BString(const char* str, size_t size);	// Initialize to the copy of the given string. Please note that `size` excludes the NULL-terminator.
	BString(const BString *str);
	BString(const BString& str);
	
	~BString();
	
	void clear();

	inline const char *set(const char *str);				// Returns the pointer that was handed to it. Also NULL if allocation failed. Passing null pointer does a no-op.
	const char        *set(const char *str, size_t len);
	const BString     *set(const BString *str);
	char    *own(char *str);					// Take ownership of the given malloced string buffer
	char    *own(char *str, size_t size);
	BString *own(BString *src);				// Take ownership of the given BString's buffer and clear the source.

	int32 length();			// This isn't size_t because otherwise we couldn't do things like -(str->length()) without casting.
	
	size_t size() const;								// Returns the size of the string in bytes, excluding the null-terminator. Not guaranteed to be the string's length in characters.
	size_t utf8_size(int32 from, int32 to);		// Returns the size of the string between the two indexes in bytes. The character at the end index is excluded (..ize(3, 5) would count characters 3 and 4)

	void split(const char *delims, BArray<BString> *out);
	
	bool        append(const char *str, size_t str_size);		// str_size = the size in bytes minus the null-terminator (like what strlen returns you)
	inline bool append(const char *str);
	inline bool append(const BString *str);
	bool        prepend(char *str, size_t str_size);
	inline bool prepend(char *str);
	inline bool prepend(BString *str);
	bool        insert(char *str, size_t str_size, int32 offset);
	inline bool insert(char *str, int32 offset);
	inline bool insert(BString *str, int32 offset);
	void        overwrite(char *str, int32 offset, int32 length = -1);
	void        overwrite(BString *str, int32 offset, int32 r_offset = 0, int32 length = -1);	// r_offset = Where to start reading from in the source string.
	void        append_c(char c);
	
	static BString *concat(BString *str1, ...);		// Concatenates the given strings to for a new string. NOTE: Remember to terminate the argument list with NULL.

	/* Formatting */
	BString *uppercase();
	BString *lowercase();

	BString    *reverse(BString *out);
	inline void reverse();
	
	static bool equals(BString *a, const char *b, size_t size);
	static bool equals(BString *a, const char *b);
	static bool equals(BString *a, BString *b);
	inline bool equals(const char *b, size_t size);
	inline bool equals(const char *b);
	inline bool equals(BString *b);
	
	bool remove(int32 start, int32 length);				// length is in chars
	bool remove(const char *start, const char *end);
	void remove_char(char chr);							// Removes all occurences of the given character
	//void remove_chars(const char *characters);		// For multiple characters, or UTF-8 characters.
	inline void resize(int32 start, int32 end, char fill_char = ' ');	// Move the start and end of the string by the given amount. Calling `resize(2, 3)` on a string would remove the first two characters, and append three characters (specified by `fill_char`) to the end. Negative integers can be used to change the direction of the operation.
	BString    *resize(int32 start, int32 end, BString *out, char fill_char = ' ');		// Returns the argument `out`, resized
	
	int32 count(char chr);					// If you want to count a UTF-8 character then you need to use ->count_chars().
	int32 count_chars(const char *chr);
	int32 index_of (char chr, const char *start = NULL);				// Returns the index of the given character or -1 if not found.
	int32 index_of_utf8 (const char *chr, const char *start = NULL);
	int32 offset_of(char chr, const char *start = NULL);				// Like ->index_of() but returns offset in bytes instead characters.
	int32 offset_of_utf8(const char *chr, const char *start = NULL);
	int32 find(const char *substr, int32 start = 0);
	
	const char *next(char chr, const char *start = NULL);		// Returns a pointer to the next occurence of the given character in the string, starting from `start` or the beginning of the string of not given.
	
	int32 offset_of_ptr(const char *str);	// Returns the index in characters of the given pointer in the string. Returns -1 if it points outside of the string.
	char *char_at(int32 index);		// NULL if out of bounds
	
	/* Returns a pointer to the internal buffer. It is recommended that you strdup() this.			*
	 * May be NULL, for example after memory errors.												*
	 * NOTE: THE ADDRESS RETURNED BY ::c_str() MAY CHANGE with calls to ::append(), ::prepend(),	*
	 * ::insert() and ::remove() because they reallocate the buffer.								*/

	inline const char *c_str();

	/* File access */
	static BString *read_file(const char *path);
	static BString *read_file(FILE *file, int32 bytes = -1, size_t chunk_size = 256);	// Creates a BString from the next n bytes in the given file. Stops at null-bytes.
	bool            write_file(const char *path);		// These return true on success and false on failure
	bool            write_file(FILE *file);

	/* Static helper functions */
	static void read_line(FILE *file, BString *str);	// Read a line of text into a string. If the string already contains text, then the text is appended. The trailing '\n' character is stripped.
		
public:
	/* Friends */
	friend int sprintf(BString *str, const char *format, ...);
	friend class BStrUtil;
};

/* Overload sprintf to allow formatted printing to BStrings */
int sprintf(BString *str, const char *format, ...) __attribute__ ((format (printf, 2, 3)));		// Print printf-formatted text to a BString. Overwrites the original string if not empty.

#include "string_inlines.h"
#endif

/*
 * size_t copy(char *out, size_t len, int32 from = 0);
 * void remove_between(int32 start, int32 end);
 * void remove_between(char *start, char *end);
 * BRefList<BString> *split(const char *split_chars);
 * void slice(uint32 start, uint32 end, BString *out);
 * void slice(uint32 start, uint32 end);
 * BString *reverse(BString *out);
 * void     reverse();
 * void delete(int32 start, int32 length);
 * void remove_char(char character);
 * void remove_chars(const char *characters);
 * void strip();
 * bool starts_with(BString *str);
 * bool ends_with(BString *str);
 * uint32 find(const char *str, size_t size, uint32 start = 0);
 * uint32 rfind(const char *str, size_t size, int32 start = -1);
 * bool serialize8(FILE *file);
 * static BString *unserialize8(FILE *file);
 * 
 */
