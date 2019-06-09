#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/listnode.h>
#include "blistnode_helpers.h"

SUITE(BListNode)
{
	TEST (test_nodes_new)
	{
		/* Check that our test helper is working */
		
		BListNode **tmp;

		BListNode *first = test_nodes_new(&tmp);

		/*REQUIRE*/
		{
			CHECK (first == tmp[0]);
			CHECK (tmp[0]->prev == NULL);
			CHECK (tmp[0]->next == tmp[1]);
			CHECK (tmp[TEST_NODES-1]->next == NULL);					// TEST_NODES defined in blistnode_helpers.h
			CHECK (tmp[TEST_NODES-1]->prev == tmp[TEST_NODES-2]);
		}

		test_nodes_free(&tmp);
	}

	TEST (last)
	{
		BListNode **tmp;

		test_nodes_new(&tmp);

		CHECK (BListNode::last(tmp[1]) == tmp[3]);
		CHECK (BListNode::last(tmp[3]) == tmp[3]);
		
		test_nodes_free(&tmp);
	}
	
	TEST (first)
	{
		BListNode **tmp;

		test_nodes_new(&tmp);

		CHECK (BListNode::first(tmp[2]) == tmp[0]);
		CHECK (BListNode::first(tmp[0]) == tmp[0]);

		test_nodes_free(&tmp);
	}	

	TEST (last_first_SingleNode)
	{
		BListNode *node = (BListNode *) calloc(1, sizeof(BListNode));		// Pointers will be NULL

		CHECK (BListNode::last(node)  == node);
		CHECK (BListNode::first(node) == node);
		
		free(node);
	}
	
	TEST (preceding)
	{
		BListNode **tmp;

		test_nodes_new(&tmp);

		CHECK (BListNode::preceding(tmp[0]) == 0);
		CHECK (BListNode::preceding(tmp[1]) == 1);
		CHECK (BListNode::preceding(tmp[TEST_NODES - 1]) == TEST_NODES - 1);

		test_nodes_free(&tmp);
	}	

	TEST (following)
	{
		BListNode **tmp;

		test_nodes_new(&tmp);

		CHECK (BListNode::following(tmp[0]) == TEST_NODES - 1);
		CHECK (BListNode::following(tmp[1]) == TEST_NODES - 2);
		CHECK (BListNode::following(tmp[TEST_NODES - 1]) == 0);

		test_nodes_free(&tmp);
	}
	
	void foreach_NoData_func(BListNode *node) {}
	
	TEST (foreach_NoData)
	{
		MockRepository mocks;
		
		BListNode **tmp;
		BListNode *first = test_nodes_new(&tmp);
		
		mocks.ExpectCallFunc(foreach_NoData_func).With(tmp[0]);
		mocks.ExpectCallFunc(foreach_NoData_func).With(tmp[1]);
		mocks.ExpectCallFunc(foreach_NoData_func).With(tmp[2]);
		mocks.ExpectCallFunc(foreach_NoData_func).With(tmp[3]);

		BListNode::foreach(first, foreach_NoData_func);

		test_nodes_free(&tmp);
	}

	TEST (length)
	{
		BListNode **tmp;

		test_nodes_new(&tmp);
		
		CHECK (BListNode::length(tmp[0]) == TEST_NODES);
		
		test_nodes_free(&tmp);
	}
	
	TEST (length_SingleNode)
	{
		BListNode *node = (BListNode *) calloc(1, sizeof(BListNode));	// In this case, the node is the only node in the list.
		
		CHECK (BListNode::length(node) == 1);
		
		free(node);
	}
	
	TEST (insert)
	{
		BListNode  *new_node = (BListNode *) calloc(1, sizeof(BListNode));		// Pointers will be NULL
		BListNode **tmp;
		
		test_nodes_new(&tmp);
		
		BListNode::insert(new_node, tmp[2]);
		
		CHECK (tmp[1]->next == new_node);
		CHECK (new_node->next == tmp[2]);
		CHECK (tmp[2]->prev == new_node);
		CHECK (new_node->prev == tmp[1]);
		
		CHECK(BListNode::length(tmp[0]) == TEST_NODES + 1);
		
		test_nodes_free(&tmp);
		free(new_node);
	}

	TEST (insert_SingleNode)
	{
		BListNode *node_a = (BListNode *) calloc(1, sizeof(BListNode));
		BListNode *node_b = (BListNode *) calloc(1, sizeof(BListNode));
		
		BListNode::insert(node_b, node_a);	// Put node b before node a
		
		/* ... check it hasn't crashed ... */
		
		CHECK (node_b->prev == NULL);

		free(node_a);
		free(node_b);
	}

	TEST (append)
	{
		BListNode *node_a = (BListNode *) calloc(1, sizeof(BListNode));
		BListNode *node_b = (BListNode *) calloc(1, sizeof(BListNode));
		
		BListNode::append(node_b, node_a);	// We append node b to the end of node a's list
		
		CHECK (node_a->next == node_b);
		CHECK (node_b->prev == node_a);

		free(node_a);
		free(node_b);
	}

	TEST (append_ListPtr)
	{
		BListNode *node_a = (BListNode *) calloc(1, sizeof(BListNode));
		BListNode *node_b = (BListNode *) calloc(1, sizeof(BListNode));
		
		BListNode *list = NULL;
		
		BListNode::append(node_a, &list);
		
		CHECK (list == node_a);
		CHECK (node_a->next == NULL);
		CHECK (node_a->prev == NULL);
		
		BListNode::append(node_b, &list);

		CHECK (list == node_a);
		CHECK (node_a->next == node_b);
		CHECK (node_a->prev == NULL);
		CHECK (node_b->next == NULL);
		CHECK (node_b->prev == node_a);

		free(node_a);
		free(node_b);
	}

	TEST (rip)
	{
		BListNode **tmp;
		
		test_nodes_new(&tmp);
		
		BListNode::rip(tmp[2]);
		
		CHECK (tmp[1]->next == tmp[3]);
		CHECK (tmp[3]->prev == tmp[1]);
		
		CHECK (tmp[2]->next == tmp[3]);	// Check that the internal pointers remain untouched
		CHECK (tmp[2]->prev == tmp[1]);
		
		test_nodes_free(&tmp);
	}
	
	TEST (rip_EdgeNodes)
	{
		/* Test whether it can deal with nodes that are at	*
		 * the edge of a list, or the only node in a list.	*/
		
		BListNode *node = (BListNode *) calloc(1, sizeof(BListNode));	// In this case, the node is the only node in the list.

		BListNode::rip(node);
		
		/* We can't really check anything here,	*
		 * just see whether the test will crash	*/
		
		free(node);
	}

	TEST (remove)
	{
		BListNode **tmp;
		
		test_nodes_new(&tmp);
		
		BListNode::remove(tmp[2]);
		
		CHECK (tmp[1]->next == tmp[3]);
		CHECK (tmp[3]->prev == tmp[1]);
		
		CHECK (tmp[2]->next == NULL);	// Check that the internal pointers have been NULLed
		CHECK (tmp[2]->prev == NULL);
		
		test_nodes_free(&tmp);
	}

	TEST (get)
	{
		BListNode **tmp;
		
		test_nodes_new(&tmp);
		
		CHECK (BListNode::get(tmp[2],  0) == tmp[2]);	// 0 should return the same node as given
		CHECK (BListNode::get(tmp[2],  1) == tmp[3]);
		CHECK (BListNode::get(tmp[2], -2) == tmp[0]);
		
		CHECK (BListNode::get(tmp[TEST_NODES - 1], 8) == NULL);
		CHECK (BListNode::get(tmp[2], -6) == NULL);
		
		test_nodes_free(&tmp);
	}

	TEST (get_SingleNode)
	{
		BListNode *node = (BListNode *) calloc(1, sizeof(BListNode));		// Pointers will be NULL

		CHECK (BListNode::get(node,  1) == NULL);
		CHECK (BListNode::get(node,  0) == node);
		CHECK (BListNode::get(node, -1) == NULL);
		
		free(node);
	}
	
	TEST (replace)
	{
		BListNode  *new_node = (BListNode *) calloc(1, sizeof(BListNode));		// Pointers will be NULL
		BListNode **tmp;
		
		test_nodes_new(&tmp);
		
		BListNode::replace(tmp[2], new_node);
		
		/* Check that the surrounding nodes'	*
		 * pointers have been changed.			*/
		CHECK (tmp[1]->next == new_node);
		CHECK (tmp[3]->prev == new_node);
		
		/* Check that the new node's pointers	*
		 * point to its new neighbors.			*/
		CHECK (new_node->next == tmp[3]);
		CHECK (new_node->prev == tmp[1]);
		
		/* Check that the removed node's		*
		 * pointers have been NULLed.			*/
		CHECK (tmp[2]->next == NULL);
		CHECK (tmp[2]->prev == NULL);
		
		test_nodes_free(&tmp);
		free(new_node);
	}

	TEST (replace_SingleNode)
	{
		/* This is to check whether it can deal with	*
		 * either or both (in this case) of the			*
		 * surrounding pointers being NULL.				*/
		
		BListNode *node_a = (BListNode *) calloc(1, sizeof(BListNode));
		BListNode *node_b = (BListNode *) calloc(1, sizeof(BListNode));
		
		BListNode::replace(node_a, node_b);
		
		/* If it hasn't already crashed then all there	*
		 * is to check is whether the old node's		*
		 * pointers are NULL.							*/
		CHECK (node_a->next == NULL);
		CHECK (node_a->prev == NULL);

		free(node_a);
		free(node_b);
	}
}
