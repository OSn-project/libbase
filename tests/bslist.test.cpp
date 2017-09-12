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
		
		BSListNode *first = (BSListNode *) malloc(sizeof(BSListNode))
		list->set_first();
		list->first_node()->data = test_str1;
		list->first_node()->next = NULL;
		
		list->add((void *) test_str2);
		
		CHECK(! strcmp((char *) list->first_node()->next->data, test_str2));
		
		delete list;
	}
}
