#ifndef __BASE_TESTS_BLISTNODE_HELPERS_H__
#define __BASE_TESTS_BLISTNODE_HELPERS_H__

#include "../include/blistnode.h"

#define TEST_NODES 4    // Don't make this less than 4 or you'll break the tests

BListNode* test_nodes_new(BListNode ***init_nodes);
void test_nodes_free(BListNode ***init_nodes);

#endif
