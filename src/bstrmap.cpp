#include <stdlib.h>
#include <string.h>

#include <base/misc.h>
#include <base/strmap.h>

BStrMap :: BStrMap(uint32 init_size)
{
	this->allocated = init_size;
	this->used = 0;
	
	this->pairs = (BStrMap::Pair *) calloc(init_size, sizeof(BStrMap::Pair));	// This automatically sets all ->name pointers to NULL, marking the pairs as free.
}

BStrMap :: ~BStrMap()
{
	free(this->pairs);
}

void BStrMap :: add(const char *name, void *data)
{
	Pair *pair = this->new_pair();
	
	pair->name = strdup(name);
	pair->data = data;
	
	this->used++;
}

bool BStrMap :: set(const char *name, void *data)
{
	Pair *pair = this->find_pair(name);
	
	if (pair == NULL)
	{
		return false;
	}
	else
	{
		pair->data = data;
		return true;
	}
}

void *BStrMap :: get(const char *name)
{
	Pair *pair = this->find_pair(name);
	
	if (pair == NULL)
	{
		return NULL;
	}
	else
	{
		return pair->data;
	}
}

BStrMap::Pair *BStrMap :: grow()
{
	uint32 new_size = ((this->allocated * 3) / 2) + 1;		// An array of size 1 would stay at 1 without the +1 bit.
	
	/* Grow the array */
	this->pairs = (Pair *) realloc(this->pairs, new_size * sizeof(Pair));
	
	/* Mark the new pairs as free */
	Pair *new_pairs = &this->pairs[this->allocated];	// The start of the newly allocated pairs
	
	for (Pair *current = new_pairs; current < &this->pairs[new_size]; current++)
	{
		Pair::mark_free(current);
	}
	
	/* Change the stored array length */
	this->allocated = new_size;
	
	return new_pairs;
}

BStrMap::Pair *BStrMap :: new_pair()
{
	if (this->used == this->allocated)
	{
		/* If there are no free pairs left, we need to grow the array. */
		return this->grow();
	}
	else
	{
		/* Find a free pair */
		return Pair::next_free(this->pairs, this->allocated);
	}
}

BStrMap::Pair *BStrMap :: find_pair(const char *name)
{
	for (uint32 i = 0; i < this->allocated; i++)
	{
		Pair *pair = &this->pairs[i];
		
		if (streq(pair->name, name))
		{
			return pair;
		}
	}
	
	return NULL;
}

BStrMap::Pair *BStrMap::Pair :: next_free(BStrMap::Pair *pairs, uint32 allocated)	// Lord this is messy
{
	for (uint32 i = 0; i < allocated; i++)
	{
		if (pairs[i].name == NULL)
			return &pairs[i];
	}
	
	return NULL;
}

void BStrMap::Pair :: mark_free(BStrMap::Pair *pair)
{
	pair->name = NULL;
}
