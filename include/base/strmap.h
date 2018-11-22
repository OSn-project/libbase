/* Simple but low-efficiency map.	*/

#ifndef __BSTRMAP_H__
#define __BSTRMAP_H__

#include <osndef.h>

class BStrMap
{
public:
	struct Pair;

private:
	Pair  *pairs;
	uint32 allocated, used;
	
public:
	BStrMap(uint32 init_size = 8);
	~BStrMap();
	
	void   add(const char *name, void *value);
	bool   set(const char *name, void *value);	// Sets the value of an existing key. Returns false if the key was not found.
	void  *get(const char *name);

private:
	Pair *grow();		// Grows the array and returns a pointer to the first new free pair.
	void  shrink();
	
	Pair *new_pair();	// Used to request a new pair. Either finds a free existing pair or grows the array if none are found.
	Pair *find_pair(const char *name);
};

struct BStrMap::Pair
{
	char *name;		// If the `name` pointer is null, the pair is free to be overwritten.
	void *data;

	static Pair   *next_free(Pair *pairs, uint32 allocated);		// Gets the next free pair or returns NULL if all pairs are used.
	static uint32 count_free(Pair *pairs, uint32 allocated);
	
	static void    mark_free(Pair *pair);
};
// set: overwrites or creates
// set_..: creates but does not overwrite
#endif
