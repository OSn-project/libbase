#include <stdio.h>
#include <stdlib.h>

#include "bptrlist_helpers.h"

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
	
	list->*member<proxy_m_length>::value = TEST_NODES;
	list->*member<proxy_first>::value  = __init_nodes[0];

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
	
	list->*member<proxy_m_length>::value = 0;
	list->*member<proxy_first>::value    = NULL;	// So that it doesn't try to free the nodes that we've just freed
	
	delete list;
}
