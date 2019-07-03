/* A couple of helper functions used by tests	*
 * dealing with BListNodes.						*/

#ifndef __BLISTNODE_TEST_HELPERS_H_
#define __BLISTNODE_TEST_HELPERS_H_

#include <base/listnode.h>

#define TEST_NODES 4

BListNode* test_nodes_new(BListNode ***init_nodes);
void test_nodes_free(BListNode ***init_nodes);

#endif
