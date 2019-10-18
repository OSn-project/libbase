#ifndef __BASE_MISC_H_
#define __BASE_MISC_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

bool streq(const char *a, const char *b);
bool strneq(const char *a, const char *b, size_t max);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
template <typename T>
inline T fread(FILE *file)
{
	T val;
	fread(&val, sizeof(T), 1, file);

	return val;
}

template <typename T>
inline void fwrite(T val, FILE *file)
{
	fwrite(&val, sizeof(T), 1, file);
}
#endif

#endif
