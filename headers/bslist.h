#ifndef __BSLIST_H__
#define __BSLIST_H__

#include "bdefs.h"

struct BSListNode
{
	void       *data;
	BSListNode *next;
};

class BSList
{	
protected:

	BSListNode  *m_first;
	int32        m_length;						// This is signed in order to be compatible with all the other integers used in the BSList
	
	BSListNode **ptr_to_index(int32 index);		// Returns a pointer to the pointer pointing to the node at the given index. (Normaly the ->next pointer of the previous node)
	
public:
		
	BSList();
	~BSList();
	
	void *get(uint32 index);
	int32 length();

	int32 index_of(void *item);		//! Returns the index of the first occurence of the given item in the list. Returns -1 if not found.
	
	/* Adding and removing */
	void add(void *item);
	void add_node(BSListNode *node);
	void insert(void *item, int32 index);
	void remove(void *item);
	void remove_at(int32 index);
	void reset();
	
	/* Slicing */
	BSList *slice(int32 start, int32 end);
	
	/* Moving */
	void move(int32 index_from, int32 index_to);
	
	/* For iterating */
	inline BSListNode *first_node() {return this->m_first;};
};

#endif
