/* Utility functions to deal with UTF-8 */

#ifndef __B_UTF8_H__
#define __B_UTF8_H__

#include "bdefs.h"

#ifdef __cplusplus
extern "C" {
#endif

int32  utf8_length(char *string);
char  *utf8_nextchar(char *chr);
char  *utf8_char_at(char *string, int32 index);						// If index is higher than the string length, a pointer to the string's null-terminator will be returned.
void   utf8_first(char *src, char *dest, int32 chars);				// Copies the first n utf8 characters into the given location
void   utf8_rest (char *src, char *dest, int32 start);				// !!!NIY!!!
void   utf8_slice(char *src, char *dest, int32 start, int32 chars);	// !!!NIY!!!

#ifdef __cplusplus
}
#endif

#endif
