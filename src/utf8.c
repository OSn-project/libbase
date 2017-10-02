#include <stdlib.h>
#include "bdefs.h"
#include "utf8.h"

int32 utf8_length(char *string)
{
	if (! string) return 0;
	uint32 length = 0;
	
	for (char *c = string; *c != '\0'; c++)
	{
		if ((*c & 0b11000000) != 0b10000000)	// If the two leftmost bits of the char aren't 10
		{
			length++;
		}
	}
	
	return length;
}

char *utf8_nextchar(char *chr)
{
	if (chr == NULL) return NULL;
	
	do
	{
		chr++;
	}
	while ((*chr & 0b11000000) == 0b10000000);	// Skip over unicode char continuation bytes
	
	return chr;
}

char *utf8_char_at(char *string, int32 index)
{
	if (index < 0) return NULL;
	
	char *chr = string;
	
	for (int32 count = 0; count < index && *chr != '\0'; count++)
	{
		chr = utf8_nextchar(chr);
	}
	
	return chr;
}

void  utf8_first(char *src, char *dest, int32 chars)
{
	if (chars <= 0)	{*(dest) = '\0'; return;}
	
	char *chr = src;
	
	for (int32 char_count = 0; char_count < chars;)
	{
		do
		{
			*dest = *chr;

			chr++, dest++;
		} while ((*chr & 0b11000000) == 0b10000000);
		
		char_count++;
	}
	
	*(dest) = '\0';		// Add a null-terminator to the end.
}

void  utf8_slice(char *src, char *dest, int32 start, int32 chars)
{
	if (chars <= 0)	{*(dest) = '\0'; return;}
	if (start <  0)	{*(dest) = '\0'; return;}
	
	char *chr = utf8_char_at(src, start);
	
	for (int32 char_count = 0; char_count < chars && *chr != '\0';)
	{
		do
		{
			*dest = *chr;

			chr++, dest++;
		} while ((*chr & 0b11000000) == 0b10000000);
		
		char_count++;
	}
	
	*(dest) = '\0';		// Add a null-terminator to the end.
}
