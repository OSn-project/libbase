#include <stdlib.h>
#include <string.h>

#include "bstring.h"
#include "bdefs.h"
#include <stdio.h>

BString :: BString ()
{
	this->string = NULL;
	this->allocated_size = 0;
}

BString :: BString (const char *c_str)
{
	this->string = strdup(c_str);
	this->allocated_size = strlen(c_str);
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
	
	size_t required_size = strlen(str) + 1;
	this->string = (char *) realloc(this->string, required_size);
	
	if (! this->string)
	{
		/* In case of an error from realloc */
		this->allocated_size = 0;
		return false;
	}
	
	/* Copy the string */
	strcpy(this->string, str);
	
	return true;
}

int32 BString :: length()
{
	if (this->string == NULL) return 0;
	
	int32 length = 0;
	
	for (char *chr = this->string; *chr != '\0'; chr++)
	{
		length++;
	}
	
	return length;
}

int32 BString :: length_utf8()
{
	char *start = this->string;
	
	uint32 length = 0;
	
	if (start == NULL) return 0;	// If it's still NULL (this->string == NULL)
	
	for (char *c = start; *c != '\0'; c++)
	{
		if ((*c & 0b11000000) != 0b10000000)	// If the two leftmost bits of the char aren't 10
		{
			length++;
		}
	}
	
	return length;

}

const char *BString :: c_str ()
{
	return (const char *) (this->string ? this->string : "");
}

/*BString *BString :: slice(int32 start, int32 length)
{
	BString *sliced = new BString();
}*/

/* UTF-8 functions */

char *BString :: __utf8_char_at(int32 index)
{
	if (index < 0) return NULL;
	
	char *chr = this->string;
	
	for (int32 count = 0; count < index; count++)
	{
		chr = this->__utf8_nextchar(chr);
	}
	
	return chr;
}

void  BString :: __utf8_first(char *dest, int32 chars)
{
	if (chars < 0) return;
	
	char *chr = this->string;
	
	for (int32 char_count = 0; char_count < chars; char_count++)
	{
		*dest = *chr;
		
		if ((*chr & 0b11000000) != 0b10000000) char_count++;		// Increment the char counter unless this is a continuation byte
		
		chr++, dest++;
	}
}

void  BString :: __utf8_rest (char *dest, int32 start)
{
	if (start < 0) return;
}

void  BString :: __utf8_slice(char *dest, int32 start, int32 chars)
{
	if (start < 0 || chars < 0) return;
	
	char *chr = this->__utf8_char_at(start);
	
	for (int32 count = 0; count < chars && *chr != '\0'; chr++, dest++)
	{
		*dest = *chr;
		
		if ((*chr & 0b11000000) != 0b10000000)
		{
			count++;
		}
	}
}

inline char *BString :: __utf8_nextchar(char *start)
{
	if (start == NULL) return NULL;
	
	do
	{
		start++;
	}
	while ((*start & 0b11000000) == 0b10000000);	// Skip over unicode char continuation bytes
	
	return start;
}

/*int main()
{
	static char *c_str[] = "✓♞☭€❄a♫₽";
	
	BString *myStr = BString(c_str, true);
	printf("%s\n", myStr->__utf8_nextchar());
	return 0;
}*/
