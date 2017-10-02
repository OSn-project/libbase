#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>

#include "../headers/bslist.h"

SUITE(BSList)
{	
	static char test_str1[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	static char test_str2[] = "abcdefghijklmnopqrstuvwxyz";
	static char test_str3[] = "1234567890";
	static char test_str4[] = "qwertyuiop";
	static char test_str5[] = "Tabs > Spaces";
	
	TEST(EmptyConstructor)
	{
		BSList *list = new BSList();
		
		CHECK_EQUAL(list->first_node(), (void*)NULL);
		
		delete list;
	}
	
	TEST(AddToEmptyList)
	{
		BSList *list = new BSList();
		
		list->add((void *) test_str1);
		
		CHECK_EQUAL(list->first_node()->data, test_str1);
		
		delete list;
	}
	
	TEST(AddToPopulatedList)
	{
		BSList *list = new BSList();
		
		BSListNode *first = (BSListNode *) malloc(sizeof(BSListNode));
		first->data = test_str1;
		list->add_node(first);
		
		list->add((void *) test_str2);
		
		CHECK(first->next != NULL);				// Check that it's actually added the new node to the list
		CHECK(first->next->data == test_str2);	// Check that it's filled the node with the data
		CHECK(first->next->next == NULL);		// Check that it's ended the list properly.
		
		delete list;
	}
}
