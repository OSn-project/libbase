#ifndef __BASE_LISTNODE_H__
#define __BASE_LISTNODE_H__

#include <stdlib.h>
#include "osndef.h"

#define BLISTNODE(P) ((struct BListNode *) P)	// This macro is intended to simplify access to the ->next and ->prev pointers of C structs with a BListNode as their first member. Use of this macro on a C++ class inheriting BListNode is discouraged as other inherited classes may come before it.

struct BListNode
{
	BListNode *next;
	BListNode *prev;

    static BListNode *last   (BListNode *node);		// Get the last node in the list.
    static BListNode *first  (BListNode *node);		// Get the first node in the list.
    static size_t     preceding(BListNode *node);	// Count the nodes preceding the given one in the list
    static size_t     following(BListNode *node);	// Count the nodes following the given one in the list
    
    static inline size_t length (BListNode *first) { return (first == NULL) ? 0 : BListNode::following(first) + 1; }  // Find the length of a linked list when given a pointer to the first node. Just a wrapper to make usage clearer.
    
    static void foreach (BListNode *first, void (*func)(BListNode *node));
    static void foreach (BListNode *first, void (*func)(BListNode *node, void *data), void *data);	// Often it is simpler to just use a normal for loop than to use these
    
    static void       insert(BListNode *add, BListNode *node);  // Insert a node before the given node in a list.
    static void       append(BListNode *add, BListNode *first);	// Add a node to the END of a list.
    static void       rip   (BListNode *node);                  // 'Rips' a node out of its list. ->prev and ->next pointers are not cleared.
    static void       remove(BListNode *node);                  // Remove a node from its list and NULL its ->next and ->prev pointers.

    static BListNode *get(BListNode *node, int32 index);	// Get the node at the index relative to this node. Negative indexes are supported. Out-of-range indexes will return NULL.
    static void       replace(BListNode *current, BListNode *target);	// Replace the given node
	
    /* Circular list functions */
    static BListNode *circ_last  (BListNode *node, BListNode *first);

    static BListNode *circ_add  (BListNode *node, BListNode *add);
};

#endif

//static void sort(BListNode *first, bool (*compare_func)(const BListNode *a, const BListNode *b));
