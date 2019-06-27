/* Simple but low-efficiency key-value map.	*/

#ifndef BDICTIONARY_H_
#define BDICTIONARY_H_

#include <osndef.h>

class BDict
{
public:
	struct Entry;

private:
	Entry *entries;
	uint32 allocated, used;
	
public:
	BDict(uint32 init_size = 8);
	~BDict();
	
	void   add(const char *name, void *value);
	bool   set(const char *name, void *value);	// Sets the value of an existing key. Returns false if the key was not found.
	void  *get(const char *name);

private:
	Entry *grow();		// Grows the array and returns a pointer to the first new free pair.
	void  shrink();
	
	Entry *new_entry();		// Used to request a new entry. Either finds a free existing pair or grows the array if none are found.
	Entry *find_entry(const char *name);
};

struct BDict::Entry
{
	char *name;		// If the `name` pointer is null, the entry is free to be overwritten.
	void *data;

	static Entry  *next_free(Entry *pairs, uint32 allocated);		// Gets the next free pair or returns NULL if all pairs are used.
	static uint32 count_free(Entry *pairs, uint32 allocated);
	
	static void   mark_free(Entry *pair);
};
// set: overwrites or creates
// set_..: creates but does not overwrite
#endif
