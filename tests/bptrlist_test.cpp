#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/ptrlist.h>

#define TEST_NODES 4    // Don't make this less than 4 or you'll break the tests

/* Privablic */

struct m_length { typedef size_t (BVoidPtrList::*type); };
template class private_member<m_length, &BVoidPtrList::m_length>;

struct first { typedef BPtrListNode *(BVoidPtrList::*type); };
template class private_member<first, &BVoidPtrList::first>;

struct last { typedef BPtrListNode *(BVoidPtrList::*type); };
template class private_member<last, &BVoidPtrList::last>;

struct append_node { typedef void(BVoidPtrList::*type)(BPtrListNode *node); };
template class private_member<append_node, &BVoidPtrList::append_node>;


SUITE(BPtrList)
{
	/* Test helpers */
	BVoidPtrList* test_list_new(BPtrListNode ***init_nodes)
	{
		/* Make a null-terminated array to containin pointers to the nodes	*
		 * we'll create, so that we can free them later.					*/
		BPtrListNode **__init_nodes = (BPtrListNode **) calloc(TEST_NODES + 1, sizeof(BPtrListNode *));

		/* Allocate the test nodes */
		for (int i = 0; i < TEST_NODES; i++)
		{
			__init_nodes[i] = (BPtrListNode *) malloc(sizeof(BPtrListNode));
		}

		/* Link the nodes */
		for (int i = 0; i < TEST_NODES; i++)
		{
			__init_nodes[i]->next = (i == TEST_NODES - 1) ? NULL : __init_nodes[i + 1];
			__init_nodes[i]->prev = (i == 0)              ? NULL : __init_nodes[i - 1];
			
			__init_nodes[i]->item = (void *) i;
		}

		*init_nodes = __init_nodes;
		
		/* Construct the list object to contain the nodes */
		
		BVoidPtrList *list = new BVoidPtrList;
		
		list->*member<m_length>::value = TEST_NODES;
		list->*member<first>::value  = __init_nodes[0];

		return list;
	}

	void test_list_free(BVoidPtrList *list, BPtrListNode ***init_nodes)
	{	
		for (BPtrListNode **current = *init_nodes; *current != NULL; current++)
		{
			/* Go down the array, freeing all the pointers */

			free (*current);
		}

		free(*init_nodes);		// Free the array itself
		
		list->*member<m_length>::value = 0;
		list->*member<first>::value    = NULL;	// So that it doesn't try to free the nodes that we've just freed
		
		delete list;
	}
	
	/* Tests */

	TEST (test_list_new)
	{
		/* Check that our test helper is working */
		
		BPtrListNode **tmp;

		BVoidPtrList *list = test_list_new(&tmp);

		/*REQUIRE*/
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
		
		/*REQUIRE*/
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

	//TEST (append_node)
	//{
		//BVoidPtrList list;
		//BPtrListNode *node1 = (BPtrListNode *) malloc(sizeof(BPtrListNode));
		//BPtrListNode *node2 = (BPtrListNode *) malloc(sizeof(BPtrListNode));
		
		///* Append to an empty list */
		//(&list->*func<append_node>::ptr)(node1);
		
		//CHECK(list.*member<m_length>::value == 1);
		
		//CHECK(list.*member<first>::value == node1);
		//CHECK(list.*member<last>::value == node1);

		///* Append to a list with other items */
		//(&list->*func<append_node>::ptr)(node2);
		
		//CHECK(list.*member<m_length>::value == 2);
		
		//CHECK(list.*member<first>::value == node1);
		//CHECK(list.*member<last>::value == node2);
	//}
}
