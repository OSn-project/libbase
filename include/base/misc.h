#ifndef __BASE_MISC_H__
#define __BASE_MISC_H__

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

#define ceil_div(N, D) (1 + ((N - 1) / D))		// https://stackoverflow.com/questions/2745074/fast-ceiling-of-an-integer-division-in-c-c

bool streq(const char *a, const char *b);
bool strneq(const char *a, const char *b, size_t max);

#ifdef __cplusplus
}
#endif

#endif
