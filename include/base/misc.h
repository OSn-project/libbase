#ifndef __BASE_MISC_H__
#define __BASE_MISC_H__

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

bool streq(const char *a, const char *b);
bool strneq(const char *a, const char *b, size_t max);

#ifdef __cplusplus
}
#endif

#endif
