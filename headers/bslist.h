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
	int32        m_length;						// This should never be less than 0. It is signed just to be compatable with all the other integers the class works with.
	
	BSListNode **ptr_to_index(int32 index);		// Returns a pointer to the pointer pointing to the node at the given index. (Normaly the ->next pointer of the previous node)
	
public:
		
	BSList();
	~BSList();
	
	void *get(uint32 index);
	int32 length() {return this->m_length;};

	int32 index_of(void *item);		//! Returns the index of the first occurence of the given item in the list. Returns -1 if not found.
	
	/* Adding and removing */
	void add(void *item);
	void insert(void *item, int32 index);
	void remove(void *item);
	void remove_at(int32 index);
	void reset();
	
	/* Slicing */
	BSList *slice(int32 start, int32 end);
	
	/* Moving */
	void move(int32 index_from, int32 index_to);
	
	/* Dangerous functions */
	inline BSListNode *first_node() {return this->m_first;}; 		// This should only be used to iterate through the list using the ->next pointer.
	inline void set_length(int32 length) {this->m_length = length;};	// However if you *do* decide to maniplulate the nodes yourself, be sure to change the length accordingly.
};

#endif
