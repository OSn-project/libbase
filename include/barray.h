/* A dynamic array of items of a set size.		*
 * Not only can it hold pointers, but it can	*
 * also hold larger structures.	It does not do	*
 * bounds checking or negative indexes -- this	*
 * is left for inheriting classes to do.		*/

// DO NOT USE WITH CLASSES THAT YOU HAVE POINTERS TO. Classes are COPIED into the 
array.
 
#ifndef __BASE_ARRAY_H__
#define __BASE_ARRAY_H__

#include <stddef.h>
//#include <osndef.h>
#include "../include/osndef.h"

class BMemArray
{
protected:
	size_t item_size;
	
	uint32 size;
	uint32 capacity;
	
	uint8 *data;

public:
	BMemArray(size_t item_size = sizeof(void *), uint32 init_capacity = 8);
	~BMemArray();
	
	inline uint32 length() { return this->size; }
	void *get(uint32 index);
	
	void  add(void *item);		// Adds (COPIES) an item to the list. Note, the pointer is to the item that will be copied into the array. If the method is given the address of a pointer, the pointer itself will be copied and NOT THE THING THAT IT POINTS TO.	
	bool  remove(uint32 index);	// Remove an item by its index
	
private:
	void grow();
};

template <typename T>
class BArray : public BMemArray
{
public:

	BArray(uint32 init_capacity = 8) : BMemArray(sizeof(T), init_capacity)
	{
	};
	
	inline T *get(int32 index)
	{
		/* Deal with negative indexes */
		if (index < 0)
			index += this->size;
		
		/* Check for out of bounds */
		if (index < 0 || index >= this->size)
			return NULL;
		
		return this->BMemArray::get(index);
	}
	
	inline void add(const T &item)
	{
		this->add((void *) &item);
	}
	
	/* Inherit overloaded methods from parent */
	
	using BMemArray::add;
};

#endif

// static BMemArray *from_static(void *array, uint32 length, size_t item_size);
