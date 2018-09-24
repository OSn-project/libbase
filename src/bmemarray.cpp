#include <stdlib.h>
#include <string.h>

//#include <base/memarray.h>
#include <base/array.h>

BMemArray :: BMemArray(size_t item_size, uint32 init_capacity)
{
	/* Initialize variables */
	this->item_size = item_size;
	
	this->m_len = 0;
	this->capacity = init_capacity;
	
	/* Allocate initial memory */
	this->data = (uint8 *) calloc(this->item_size, init_capacity);
}

BMemArray :: ~BMemArray()
{
	/* Free memory */
	free(this->data);
}

void *BMemArray :: get_ptr(uint32 index)
{
	return &this->data[index * this->item_size];
}

void BMemArray :: clear()
{
	this->m_len = 0;
	
	this->capacity = 8;
	this->data = (uint8 *) realloc(this->data, this->capacity * this->item_size);
}

void BMemArray :: add(void *item)
{
	/* Grow if we've run out of room */
	if (this->m_len == this->capacity)
	{
		this->grow();
	}
	
	/* Copy the item in */
	uint8 *dest = &this->data[this->m_len * this->item_size];
	memcpy(dest, item, this->item_size);	// !!! Doesn't copy correctly..?
	
	this->m_len++;
}

void *BMemArray :: add_new()
{
	/* Grow if we've run out of room */
	if (this->m_len == this->capacity)
	{
		this->grow();
	}
	
	void *item = &this->data[this->m_len * this->item_size];
	
	this->m_len++;
	
	return item;
}

void BMemArray :: remove(uint32 index)
{
	void *rem = this->get_ptr(index);
	
	memmove(rem, (uint8 *) rem + item_size, this->item_size * this->m_len - 1);
	
	this->m_len--;
	
	/* Shrink if our capacity is excessively large */
	if ((this->m_len * 3) / 2 <= this->capacity)
	{
		this->data = (uint8 *) realloc(this->data, (this->m_len * 5) / 4);	// Shrink to 5/4 the occupied size
	}
}

void *BMemArray :: find(bool (*find_func)(void *item))
{
	for (void *item = this->data; item < this->data + (this->item_size * this->m_len); item += this->item_size)
	{
		if (find_func(item) == true)
			return item;
	}
	
	/* If not found */
	return NULL;
}

void *BMemArray :: find(bool (*find_func)(void *item, void *data), void *data)
{
	for (void *item = this->data; item < this->data + (this->item_size * this->m_len); item += this->item_size)
	{
		if (find_func(item, data) == true)
			return item;
	}
	
	/* If not found */
	return NULL;
}

BMemArray *BMemArray :: from_static(void *array, uint32 length, size_t item_size)
{
	BMemArray *arr = new BMemArray(item_size, length);	// If we give it the correct initial length, the only thing we need to do is copy the data.
	
	memcpy(arr->data, array, length * item_size);
	arr->m_len = length;									// The array had a filled length of 0 when it was created (we only gave it the length to tell it how much memory to allocate). Set the length to reflect the data that we just copied into it.
	
	return arr;
}

void BMemArray :: grow()
{
	this->capacity = ((this->capacity * 3) / 2) + 1;		// Grow by 3/2 (https://en.wikipedia.org/wiki/Dynamic_array#Growth_factor)
	
	this->data = (uint8 *) realloc(this->data, this->capacity * this->item_size);
}
