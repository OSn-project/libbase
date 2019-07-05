/* Compile: clang++ -g -o bstring_split bstring_split.cpp ../../src/libbase.a -isystem ../../include/ */

#include <stdio.h>

#include <base/string.h>
#include <base/array.h>

static void piece_iter(BString& piece)
{
	puts(piece.c_str());
}

int main()
{
	printf("Enter string to split (delimiter is '.' and '\u2056'): ");
	
	BString str;
	BString::read_line(stdin, &str);
	
	BArray<BString> pieces;
	str.split(".⁖", &pieces);

	pieces.foreach(piece_iter);

	return 0;
}
