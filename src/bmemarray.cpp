#include <stdlib.h>
#include <string.h>

//#include <base/memarray.h>
#include "../include/barray.h"

BMemArray :: BMemArray(size_t item_size, uint32 init_capacity)
{
	/* Initialize variables */
	this->item_size = item_size;
	
	this->size = 0;
	this->capacity = init_capacity;
	
	/* Allocate initial memory */
	this->data = (uint8 *) calloc(this->item_size, init_capacity);
}

BMemArray :: ~BMemArray()
{
	/* Free memory */
	free(this->data);
}

void *BMemArray :: get(int32 index)
{
	return &this->data[index * this->item_size];
}

void BMemArray :: add(void *item)
{
	/* Grow if we've run out of room */
	if (this->size == this->capacity)
	{
		this->grow();
	}
	
	/* Copy the item in */
	uint8 *dest = &this->data[this->size * this->item_size];
	memcpy(dest, item, this->item_size);
	
	this->size++;
}

bool BMemArray :: remove(int32 index)
{
	void *rem = this->get(index);
	
	memmove(rem, rem + item_size, this->item_size);
	
	this->size--;
	
	/* Shrink if out capacity is excessively large */
	if ((this->size * 3) / 2 <= this->capacity)
	{
		this->data = (uint8 *) realloc(this->data, (this->size * 5) / 4);	// Shrink to 5/4 the occupied size
	}
	
	return true;
}

void BMemArray :: grow()
{
	this->capacity = (this->capacity * 3) / 2;		// Grow by 3/2 (https://en.wikipedia.org/wiki/Dynamic_array#Growth_factor)
	
	this->data = (uint8 *) realloc(this->data, this->capacity * this->item_size);
}
