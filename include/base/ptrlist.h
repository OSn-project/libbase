/* This is a class that maintains a linked list of pointers.	*
 * The benefit of linking pointers to objects rather than the	*
 * objects themselves is that in the former case the objects	*
 * can be part of multiple lists.								*
 * 																*
 * In order to use as few templates as possible, the actual		*
 * list manipulation is done on void pointers in the			*
 * BVoidPtrList class, which is then sub-classed by the BPtrList*
 * class which consists exclusively of inline functions that do	*
 * casting.														*/

#ifndef __BASE_PTRLIST_H__
#define __BASE_PTRLIST_H__

#include <base/listnode.h>

struct BPtrListNode : BListNode
{
	/* We inherit the ->next and ->prev pointers from BListNode,	*
	 * and in addition to that we store a pointer to the list item.	*/

	void *item;	
};

class BVoidPtrList
{
protected:
	size_t m_length;		// m_ to prevent name clash (signifies that it is private)

	BPtrListNode *first, *last;

public:
	BVoidPtrList();
	~BVoidPtrList();
	
	size_t length();
	void clear();

	void append(void *item);
	
	inline BPtrListNode *first_node() { return this->first; };
	inline BPtrListNode *last_node()  { return this->last;  };

protected:
	void append_node(BPtrListNode *node);	// Append a pre-filled list node to the list
};

/* Template class */

template <typename T>
class BPtrList : BVoidPtrList
{
public:
	
	inline void add (T *item) { this->add((void *) item); }
};

#endif
