/* Compile: clang++ -o list1 list1.cpp ../../src/libbase.a -isystem ../../include/ */

#include <stdio.h>

#include <base/string.h>
#include <base/array.h>

int main()
{
	BString input;
	BArray<BString *> items;
	
	printf("Shopping list. Type names of items to add them.\nOther commands: list, quit\n");
	
	for (bool run = true; run; input.clear())
	{
		printf(" > ");
		
		BString::read_line(stdin, &input);
		
		if (input.equals("quit"))
		{
			run = false;
		}
		else if (input.equals("list"))
		{
			for (uint32 i = 0; i < items.length(); i++)
			{
				printf(" - %s\n", items.get(i)->c_str());
			}
		}
		else
		{
			BString *item = new BString(input);
			items.add(item);
		}
	}
	
	/* Delete all strings in the list */
	for (uint32 i = 0; i < items.length(); i++)
	{
		delete items.get(i);
	}
	
	return 0;
}
