#include <base/dict.h>
#include <stdlib.h>
#include <string.h>

#include <base/misc.h>

BBaseDict :: BBaseDict(uint32 init_size)
{
	this->allocated = init_size;
	this->used = 0;
	
	this->entries = (BDictEntry *) calloc(init_size, sizeof(BDictEntry));	// This automatically sets all ->name pointers to NULL, marking the pairs as free.
}

BBaseDict :: ~BBaseDict()
{
	free(this->entries);
}

void BBaseDict :: add(const char *name, void *data)
{
	BDictEntry *pair = this->new_entry();
	
	pair->name = strdup(name);
	pair->data = data;
	
	this->used++;
}

bool BBaseDict :: set(const char *name, void *data)
{
	BDictEntry *pair = this->find_entry(name);
	
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

void *BBaseDict :: get(const char *name)
{
	BDictEntry *pair = this->find_entry(name);
	
	if (pair == NULL)
		return NULL;
	else
		return pair->data;
}

void BBaseDict :: foreach(void (*iter_func)(const char *key, void *value))
{
	for (uint32 i = 0; i < this->allocated; i++)
	{
		BDictEntry *pair = &this->entries[i];

		if (pair->name != NULL)
		{
			iter_func(pair->name, pair->data);
		}
	}
}

void BBaseDict :: foreach(void (*iter_func)(const char *key, void *value, void *data), void *data)
{
	for (uint32 i = 0; i < this->allocated; i++)
	{
		BDictEntry *pair = &this->entries[i];

		if (pair->name != NULL)
		{
			iter_func(pair->name, pair->data, data);
		}
	}
}

BDictEntry *BBaseDict :: grow()
{
	uint32 new_size = ((this->allocated * 3) / 2) + 1;		// An array of size 1 would stay at 1 without the +1 bit.
	
	/* Grow the array */
	this->entries = (BDictEntry *) realloc(this->entries, new_size * sizeof(BDictEntry));
	
	/* Mark the new pairs as free */
	BDictEntry *new_pairs = &this->entries[this->allocated];	// The start of the newly allocated pairs
	
	for (BDictEntry *current = new_pairs; current < &this->entries[new_size]; current++)
	{
		BDictEntry::mark_free(current);
	}
	
	/* Change the stored array length */
	this->allocated = new_size;
	
	return new_pairs;
}

BDictEntry *BBaseDict :: new_entry()
{
	if (this->used == this->allocated)
	{
		/* If there are no free pairs left, we need to grow the array. */
		return this->grow();
	}
	else
	{
		/* Find a free pair */
		return BDictEntry::next_free(this->entries, this->allocated);
	}
}

BDictEntry *BBaseDict :: find_entry(const char *name)
{
	for (uint32 i = 0; i < this->allocated; i++)
	{
		BDictEntry *pair = &this->entries[i];
		
		if (streq(pair->name, name))
		{
			return pair;
		}
	}
	
	return NULL;
}

BDictEntry *BDictEntry :: next_free(BDictEntry *pairs, uint32 allocated)	// Lord this is messy
{
	for (uint32 i = 0; i < allocated; i++)
	{
		if (pairs[i].name == NULL)
			return &pairs[i];
	}
	
	return NULL;
}

void BDictEntry :: mark_free(BDictEntry *pair)
{
	pair->name = NULL;
}
