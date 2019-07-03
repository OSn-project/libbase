#ifndef BLIST_H_
#define BLIST_H_

#include <osndef.h>
#include "listnode.h"

class BBaseList
{
public:
	BListNode *m_first;
	uint32     m_length;

public:
	BBaseList();
	BBaseList(BListNode *initial);

	BListNode *get(uint32 index);
	int32 index_of(BListNode *item);		// Returns the index of the given item in the list. Returns -1 if not found.

	/* Adding and removing */
	void add(BListNode *item);
	void insert(BListNode *item, uint32 index);
	BListNode *remove(uint32 index);
};

template <typename NodeType>
class BList : private BBaseList
{
public:
	/* Type-safe methods that override their BNodeList counterparts */

	inline BList() : BBaseList() {}
	inline BList(NodeType *initial) : BBaseList((BListNode *) initial) {}

	void delete_all();	// Items are not deleted automatically as they may be referenced elsewhere.

	/* Return private variables */
	inline uint32 length() { return this->m_length; }
	inline NodeType *first()  { return (NodeType *) this->m_first; }

	inline NodeType *get(uint32 index) 					{ return (NodeType *) this->BBaseList::get(index); }
	inline int32 index_of(NodeType *node)				{ return this->BBaseList::index_of((BListNode *) node); }

	/* Adding and removing */
	inline void add(NodeType *item)						{ this->BBaseList::add((BListNode *) item); }
	inline void insert(NodeType *item, uint32 index)	{ this->BBaseList::insert((BListNode *) item, index); }
	inline NodeType *remove(uint32 index)				{ return (NodeType *) this->BBaseList::remove(index); }
};

template <typename NodeType>
void BList<NodeType> :: delete_all()
{
	for (NodeType *node = (NodeType *) this->m_first, *next; node != NULL; node = next)
	{
		next = (NodeType *) node->next;
		delete node;
	}
}

#endif
