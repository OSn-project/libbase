#ifndef __BSTRUTIL_H_
#define __BSTRUTIL_H_

#include <osndef.h>

#include "string.h"

template <typename T>
class BDict;

class BStrUtil
{
public:
	static BString *escape(const BString *str);
	static BString *unescape(const BString *str);

	/* These functions fill template strings.					*
	 * The start and end of the template (eg. "$(" and ")")		*
	 * is passed as an argument. The user needs to provide a	*
	 * callback which will be called with a template's contents	*
	 * every time one is encountered, and is expected to fill	*
	 * the given string with the text to substitute the template*
	 * with. (Returns true on success or false on failure).		*
	 * There is a version that retrieves template values from a	*
	 * specified dictionary for convenience. The result is 		*
	 * stored in the string pointed to by `out`, or the source	*
	 * string if it is NULL.									*/

	static BString *fill_templ(BString *src, const char *t_open, const char *t_close, bool (*get_val)(const char *key, void *data, BString *out), void *data = NULL, BString *out = NULL);

	/* Template version which allows you to set the type of the callback's custom data argument. */
	template <typename C>
	static inline BString *fill_templ(BString *src, const char *t_open, const char *t_close, bool (*get_val)(const char *key, C data, BString *out), C data = NULL, BString *out = NULL);

	/* Convenience wrappers that retrieve values from a dictionary */
	static BString *fill_templ(BString *src, const char *t_open, const char *t_close, BDict<const char *> *values, BString *out = NULL);
	static BString *fill_templ(BString *src, const char *t_open, const char *t_close, BDict<BString *> *values, BString *out = NULL);
};

template <typename C>
inline BString *BStrUtil :: fill_templ(BString *src, const char *t_open, const char *t_close, bool (*get_val)(const char *key, C data, BString *out), C data, BString *out)
{
	return BStrUtil::fill_templ(src, t_open, t_close, (bool (*)(const char *, void *, BString *)) get_val, (void *) data, out);
}

#endif
