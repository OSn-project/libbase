#include <stdlib.h>
#include <string.h>
#include <UnitTest++/UnitTest++.h>
#define  NO_HIPPOMOCKS_NAMESPACE
#include <HippoMocks/hippomocks.h>
#include "privablic.h"

#include <base/nodelist.h>
#include "blistnode_helpers.h"

SUITE(BNodeList)
{
	TEST(constructor)
	{
		BNodeList list;
		
		CHECK(list.first  == NULL);
		CHECK(list.length == 0);
	}
	
	TEST(constructor_WithInitial)
	{
		BListNode **tmp;
		test_nodes_new(&tmp);
		
		BNodeList list(tmp[0]);
		
		CHECK(list.first  == tmp[0]);
		CHECK(list.length == TEST_NODES);
		
		test_nodes_free(&tmp);
	}
	
	TEST(get)
	{
		MockRepository mocks;
		BListNode **tmp;
		test_nodes_new(&tmp);
		BNodeList list(tmp[0]);
		
		{
			BListNode *rc;
			
			rc = list.get(2);
			CHECK(rc == tmp[2]);
			rc = list.get(177);
			CHECK(rc == NULL);
		}
		
		test_nodes_free(&tmp);
	}

	TEST(index_of)
	{
		BListNode **tmp;
		test_nodes_new(&tmp);
		BNodeList list(tmp[0]);

		CHECK(list.index_of(tmp[2]) == 2);
		CHECK(list.index_of((BListNode *) 47) == -1);
		CHECK(list.index_of(NULL)  == -1);

		test_nodes_free(&tmp);
	}

	TEST(insert)
	{
		BListNode **tmp;
		test_nodes_new(&tmp);
		BNodeList list(tmp[0]);

		BListNode new_node;
		list.insert(&new_node, 3);

		CHECK(new_node.prev == tmp[2]);
		CHECK(new_node.next == tmp[3]);
		CHECK(tmp[2]->next == &new_node);
		CHECK(tmp[3]->prev == &new_node);

		CHECK(list.length == 5);

		test_nodes_free(&tmp);
	}

	TEST(insert_start)
	{
		MockRepository mocks;
		BListNode **tmp;
		test_nodes_new(&tmp);
		BNodeList list(tmp[0]);

		BListNode new_node = {NULL, NULL};
		list.insert(&new_node, 0);

		CHECK(new_node.next == tmp[0]);
		CHECK(tmp[0]->prev == &new_node);

		CHECK(list.length == 5);

		test_nodes_free(&tmp);
	}

	TEST(insert_outOfBounds)	// TODO
	{
	}
}
