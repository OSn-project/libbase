#include <stdlib.h>
#include <string.h>

#include "bstring.h"
#include "bdefs.h"
#include "utf8.h"

BString :: BString ()
{
	this->string = NULL;
}

BString :: BString (const char *c_str)
{
	this->string = strdup(c_str);
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
