#ifndef __BASE_TESTS_BLISTNODE_HELPERS_H__
#define __BASE_TESTS_BLISTNODE_HELPERS_H__

#include "../include/bptrlist.h"

#define TEST_NODES 4    // Don't make this less than 4 or you'll break the tests

BVoidPtrList* test_list_new(BPtrListNode ***init_nodes);
void test_list_free(BVoidPtrList *list, BPtrListNode ***init_nodes);	// Deletes a test list and its originally allocated nodes. Any nodes added prior to test_list_new() will need to be freed before this.

/* Privablic */

#include "privablic.h"	// This lets us access private members of BVoidPtrList

struct m_length { typedef size_t (BVoidPtrList::*type); };
template class private_member<m_length, &BVoidPtrList::m_length>;

struct first { typedef BPtrListNode *(BVoidPtrList::*type); };
template class private_member<first, &BVoidPtrList::first>;

struct last { typedef BPtrListNode *(BVoidPtrList::*type); };
template class private_member<last, &BVoidPtrList::last>;

/* Private functions */
struct append_node { typedef void(BVoidPtrList::*type)(BPtrListNode *node); };
template class private_member<append_node, &BVoidPtrList::append_node>;

#endif
