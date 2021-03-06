/* A dynamic array of items of a set size.		*
 * Not only can it hold pointers, but it can	*
 * also hold larger structures.	It does not do	*
 * bounds checking or negative indexes -- this	*
 * is left for inheriting classes to do.		*/

// DO NOT USE WITH CLASSES THAT YOU HAVE POINTERS TO. Classes are COPIED into the array and so their address will change.
// The addresses of elements WILL change as items are inserted and removed.
//https://oopscenities.net/2011/04/30/c-inheritance-on-template-specialization/
#ifndef BARRAY_H_
#define BARRAY_H_

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
	BMemArray(size_t item_size = sizeof(void *));
	~BMemArray();
	
	inline void *get_data() { return this->data; };
	void *get_ptr(uint32 index);
	
	inline uint32 length() { return this->m_len; }
	void clear();
	
	/* Manipulation */
	void  add(void *item);		// Adds (COPIES) an item to the list. Note, the pointer is to the item that will be copied into the array. If the method is given the address of a pointer, the pointer itself will be copied and NOT THE THING THAT IT POINTS TO.
	void *add_new();			// Append memory for a new item and return a pointer to it so that it can be filled/initialized.
	void  remove(uint32 index);	// Remove an item by its index
	
//	void extend(BMemArray *other);
//	static BMemArray *concatenate(BMemArray *arr1, BMemArray *arr2);

	/* Iteration */
	void  foreach(void (*iter_func)(void *item));
	void  foreach(void (*iter_func)(void *item, void *data), void *data);	// Custom data can be passed using the `data` argument.
	void *find(bool (*find_func)(void *item));
	void *find(bool (*find_func)(void *item, void *data), void *data);	// Returns the first item for which the provided function returns true or NULL.
	
	/* Working with static arrays */
	static BMemArray *from_static(void *array, uint32 length, size_t item_size);
	void write(void *out);
	void write(void *out, size_t limit);
	
private:
	void grow();
};

template <typename T>
class BArray : public BMemArray
{
public:
	
	inline BArray() : BMemArray(sizeof(T))
	{
	}
	
	~BArray()
	{
		/* Call the destructor on all elements in the array.	*
		 * Does nothing for built-in types.						*/

		for (T *current = (T *) this->data; current < (T *) this->data + this->m_len; current++)
		{
			current->~T();
		}
	}

	inline T& get(uint32 index)
	{
		return ((T *) this->data)[index];		// It isn't possible to return a NULL-reference to indicate out-of-bounds, so requesting a bad index would just crash.
	}

	void clear()
	{
		/* We need to destruct all objects before we can free them. */
		for (T *current = (T *) this->data; current < (T *) this->data + this->m_len; current++)
		{
			current->~T();
		}

		this->BMemArray::clear();
	}

	inline void add(T const& item)		// This is a const reference. It means that we can be passed literals, ie. array->add(5);
	{
		this->BMemArray::add((void *) &item);
	}
		
	inline T *add_new()
	{
		return (T *) this->BMemArray::add_new();
	}

	inline void remove(uint32 index)
	{
		((T *) this->data)[index].~T();	// Call the destructor first

		this->BMemArray::remove(index);
	}

	inline void foreach(void (*iter_func)(T& item))
	{
		this->BMemArray::foreach((void (*)(void *)) iter_func);
	}

	template <typename C>		// Template for the custom argument. Remember that it is cast to `void *`.
	inline void foreach(void (*iter_func)(T& item, C data), C data)
	{
		this->BMemArray::foreach((void (*)(void *, void *)) iter_func, (void *) data);
	}

	inline T& find(bool (*find_func)(T& item))
	{
		return *((T *) this->find((bool (*)(void *)) find_func));	// Returns void pointer. We need to cast it to a typed pointer before we can dereference it. Once dereferenced it's returned as T&.
	}

	template <typename C>		// Template for the custom argument. Remember that it is cast to `void *`.
	inline T& find(bool (*find_func)(T& item, C data), C data)
	{
		return *((T *) this->BMemArray::find((bool (*)(void *, void *)) find_func, (void *) data));
	}
};

#endif
