#include <base/listnode.h>
#include <stdlib.h>

#include "blistnode_helpers.h"

BListNode* test_nodes_new(BListNode ***init_nodes)
{
	/* This is a test function used to create and link 4 BListNodes	*
	 * for us to test functions on. It returns a pointer to the		*
	 * first node in the list. You also need to pass this function	*
	 * an pointer to an uninitialised BListNode **, which it sets	*
	 * to point to a NULL-terminated array of pointers to the		*
	 * initial nodes, so that it can then free all of them even if	*
	 * they've been removed from the list.							*/

	/* Make a null-terminated array to containin pointers to the nodes	*
	 * we'll create, so that we can free them later.					*/
	BListNode **__init_nodes = (BListNode **) calloc(TEST_NODES + 1, sizeof(BListNode *));

	/* Allocate the test nodes */
	for (int i = 0; i < TEST_NODES; i++)
	{
		__init_nodes[i] = (BListNode *) malloc(sizeof(BListNode));
	}

	/* Link the nodes */
	for (int i = 0; i < TEST_NODES; i++)
	{
		__init_nodes[i]->next = (i == TEST_NODES - 1) ? NULL : __init_nodes[i + 1];
		__init_nodes[i]->prev = (i == 0)              ? NULL : __init_nodes[i - 1];
	}

	*init_nodes = __init_nodes;

	return __init_nodes[0];
}

void test_nodes_free(BListNode ***init_nodes)
{
	for (BListNode **current = *init_nodes; *current != NULL; current++)
	{
		/* Go down the array, freeing all the pointers */

		free (*current);
	}

	free(*init_nodes);		// Free the array itself
}
