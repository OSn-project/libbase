/* A dynamic array of items of a set size.		*
 * Not only can it hold pointers, but it can	*
 * also hold larger structures.	It does not do	*
 * bounds checking or negative indexes -- this	*
 * is left for inheriting classes to do.		*/

// DO NOT USE WITH CLASSES THAT YOU HAVE POINTERS TO. Classes are COPIED into the array and so their address will change.
// The addresses of elements WILL change as items are inserrted and removed.

#ifndef __BASE_ARRAY_H__
#define __BASE_ARRAY_H__

#include <stddef.h>
#include <osndef.h>

class BMemArray
{
protected:
	size_t item_size;
	
	uint32 m_len;		// Length of array. Can't be called `length` so that we can have a ->length() method.
	uint32 capacity;
	
	uint8 *data;

public:
	BMemArray(size_t item_size = sizeof(void *), uint32 init_capacity = 8);
	~BMemArray();
	
	inline void *get_data() { return this->data; };
	void *get_ptr(uint32 index);
	
	inline uint32 length() { return this->m_len; }
	void clear();
	
	/* Manipulation */
	void  add(void *item);		// Adds (COPIES) an item to the list. Note, the pointer is to the item that will be copied into the array. If the method is given the address of a pointer, the pointer itself will be copied and NOT THE THING THAT IT POINTS TO.
	void *add_new();			// Append memory for a new item and return a pointer to it so that it can be filled/initialized.
	void  remove(uint32 index);	// Remove an item by its index
	
	/* Working with static arrays */
	static BMemArray *from_static(void *array, uint32 length, size_t item_size);
	void write(void *out);
	void write(void *out, size_t limit);//
	
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
	
	~BArray()
	{
		for (T *current = (T *) this->data; current - (T *) this->data < this->m_len; current++)
		{
			current->~T();
		}
	}
	
	inline T& get(uint32 index)
	{
		return ((T*) this->data)[index];		// It isn't possible to return a NULL-reference to indicate out-of-bounds, so requesting a bad index would just crash.
	}

	inline void add(T const& item)				// The `const&` makes it an immutable reference, meaning that we can even be passed literals. https://stackoverflow.com/a/7701261/6130358
	{
		this->BMemArray::add((void *) &item);
	}
	
	inline T *add_new()
	{
		return (T *) this->BMemArray::add_new();
	}
	
	inline void remove(uint32 index)
	{
		((T *) this->get_ptr(index))->~T();		// Call the destructor on the item without freeing the memory.
		
		this->BMemArray::remove(index);
	}
};

#endif
