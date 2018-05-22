#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>

#include "../include/bptrlist.h"
#include "bptrlist_helpers.h"

SUITE(BPtrList)
{
	TEST (test_list_new)
	{
		/* Check that our test helper is working */
		
		BPtrListNode **tmp;

		BVoidPtrList *list = test_list_new(&tmp);

		REQUIRE
		{
			CHECK (tmp[3]->item == (void *) 3);
			
			CHECK (list->*member<first>::value == tmp[0]);
			CHECK (tmp[0]->prev == NULL);
			CHECK (tmp[0]->next == tmp[1]);
			CHECK (tmp[TEST_NODES-1]->next == NULL);					// test_list defined in BPtrListNode_helpers.h
			CHECK (tmp[TEST_NODES-1]->prev == tmp[TEST_NODES-2]);
		}

		test_list_free(list, &tmp);
	}

	TEST (constructor)
	{
		BVoidPtrList list;
		
		REQUIRE
		{
			CHECK(list.*member<m_length>::value == 0);
			
			CHECK(list.*member<first>::value == NULL);
			CHECK(list.*member<last>::value == NULL);
		}
	}
	
	/*TEST (clear)
	{
		MockRepository mocks;
		
		BPtrListNode **tmp;
		BVoidPtrList *list = test_list_new(&tmp);
			
		mocks.autoExpect = false;
    
		mocks.ExpectCallFunc(free).With(list->*member<first>::value);	// Fails for some reason
		mocks.ExpectCallFunc(free).With(tmp[1]);
		mocks.ExpectCallFunc(free).With(tmp[2]);
		mocks.ExpectCallFunc(free).With(tmp[3]);
		
		list->clear();
		
		CHECK (list->*member<m_length>::value == 0);

		CHECK(list.*member<first>::value == NULL);
		CHECK(list.*member<last>::value == NULL);
		
		delete list;	// Calling test_list_free would crash because tmp[0] .. tmp[3] have already been freed by ->clear();
		free(tmp);
	}*/

	TEST (append_node)
	{
		BVoidPtrList list;
		BPtrListNode *node1 = (BPtrListNode *) malloc(sizeof(BPtrListNode));
		BPtrListNode *node2 = (BPtrListNode *) malloc(sizeof(BPtrListNode));
		
		/* Append to an empty list */
		(&list->*func<append_node>::ptr)(node1);
		
		CHECK(list.*member<m_length>::value == 1);
		
		CHECK(list.*member<first>::value == node1);
		CHECK(list.*member<last>::value == node1);

		/* Append to a list with other items */
		(&list->*func<append_node>::ptr)(node2);
		
		CHECK(list.*member<m_length>::value == 2);
		
		CHECK(list.*member<first>::value == node1);
		CHECK(list.*member<last>::value == node2);
	}
}
