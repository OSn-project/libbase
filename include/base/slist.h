#ifndef __BPTRLIST_H_
#define __BPTRLIST_H_

#include "bdefs.h"
#include "blistnode.h"
//
//struct BPtrListNode : BListNode<BPtrListNode>
//{
//	/* The next and prev pointers are	*
//	 * inherited from BListNode.		*/
//
//	void *ptr;
//};
//
//class BPtrList
//{
//protected:
//
//	BPtrListNode  *m_first;
//	int32          m_length;						// This is signed in order to be compatible with all the other integers used in the BPtrList
//
//	BPtrListNode **ptr_to_index(int32 index);		// Returns a pointer to the pointer pointing to the node at the given index. (Normaly the ->next pointer of the previous node)
//
//private:
//	uint32_t __reserved1[4];
//
//public:
//
//	BPtrList();
//	~BPtrList();
//
//	void *get(uint32 index);
//	int32 length();
//
//	int32 index_of(void *item);		//! Returns the index of the first occurence of the given item in the list. Returns -1 if not found.
//
//	/* Adding and removing */
//	void add(void *item);
//	void add_node(BPtrListNode *node);
//	void insert(void *item, int32 index);
//	void remove(void *item);
//	void remove_at(int32 index);
//	void reset();
//
//	/* Slicing */
//	BPtrList *slice(int32 start, int32 end);
//
//	/* Moving */
//	void move(int32 index_from, int32 index_to);
//
//	/* For iterating */
//	inline BPtrListNode *first_node() {return this->m_first;};
//};

#endif
