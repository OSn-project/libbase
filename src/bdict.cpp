#include <base/dict.h>
#include <stdlib.h>
#include <string.h>

#include <base/misc.h>

BDict :: BDict(uint32 init_size)
{
	this->allocated = init_size;
	this->used = 0;
	
	this->entries = (BDict::Entry *) calloc(init_size, sizeof(BDict::Entry));	// This automatically sets all ->name pointers to NULL, marking the pairs as free.
}

BDict :: ~BDict()
{
	free(this->entries);
}

void BDict :: add(const char *name, void *data)
{
	Entry *pair = this->new_entry();
	
	pair->name = strdup(name);
	pair->data = data;
	
	this->used++;
}

bool BDict :: set(const char *name, void *data)
{
	Entry *pair = this->find_entry(name);
	
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

void *BDict :: get(const char *name)
{
	Entry *pair = this->find_entry(name);
	
	if (pair == NULL)
	{
		return NULL;
	}
	else
	{
		return pair->data;
	}
}

BDict::Entry *BDict :: grow()
{
	uint32 new_size = ((this->allocated * 3) / 2) + 1;		// An array of size 1 would stay at 1 without the +1 bit.
	
	/* Grow the array */
	this->entries = (Entry *) realloc(this->entries, new_size * sizeof(Entry));
	
	/* Mark the new pairs as free */
	Entry *new_pairs = &this->entries[this->allocated];	// The start of the newly allocated pairs
	
	for (Entry *current = new_pairs; current < &this->entries[new_size]; current++)
	{
		Entry::mark_free(current);
	}
	
	/* Change the stored array length */
	this->allocated = new_size;
	
	return new_pairs;
}

BDict::Entry *BDict :: new_entry()
{
	if (this->used == this->allocated)
	{
		/* If there are no free pairs left, we need to grow the array. */
		return this->grow();
	}
	else
	{
		/* Find a free pair */
		return Entry::next_free(this->entries, this->allocated);
	}
}

BDict::Entry *BDict :: find_entry(const char *name)
{
	for (uint32 i = 0; i < this->allocated; i++)
	{
		Entry *pair = &this->entries[i];
		
		if (streq(pair->name, name))
		{
			return pair;
		}
	}
	
	return NULL;
}

BDict::Entry *BDict::Entry :: next_free(BDict::Entry *pairs, uint32 allocated)	// Lord this is messy
{
	for (uint32 i = 0; i < allocated; i++)
	{
		if (pairs[i].name == NULL)
			return &pairs[i];
	}
	
	return NULL;
}

void BDict::Entry :: mark_free(BDict::Entry *pair)
{
	pair->name = NULL;
}
