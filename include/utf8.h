/* Utility functions to deal with UTF-8 */

#ifndef _BASE_UTF8_H
#define _BASE_UTF8_H

#include <stdbool.h>
#include "osndef.h"

#ifdef __cplusplus
extern "C" {
#endif

bool   utf8_charcmp(const char *a, const char *b);					// Compares the two UTF-8 characters pointed to
int32  utf8_length(const char *string);
char  *utf8_nextchar(char *chr);
char  *utf8_char_at(char *string, int32 index);							// If index is higher than the string length, a pointer to the string's null-terminator will be returned.
void   utf8_slice(char *src, char *dest, int32 start, int32 length);	//

char *strchr_utf8(char *str, const char *chr);

#ifdef __cplusplus
}
#endif

#endif
