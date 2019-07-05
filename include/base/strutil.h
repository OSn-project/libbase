#ifndef __BSTRUTIL_H_
#define __BSTRUTIL_H_

#include <osndef.h>

#include "string.h"

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

	static BString *fill_templ(BString *src, bool (*get_val)(const char *key, void *data, BString *out), const char *t_open, const char *t_close, void *data = NULL, BString *out = NULL);
	static BString *fill_templ(BString *src, BDict /* const char * */ *values, const char *t_open, const char *t_close, BString *out = NULL);

	template <typename C>
	static inline BString *fill_templ(BString *src, bool (*get_val)(const char *key, C data, BString *out), const char *t_open, const char *t_close, C data = NULL, BString *out = NULL)
	{
		return BStrUtil::fill_templ(src, (bool (*)(const char *, void *, BString *)) get_val, t_open, t_close, (void *) data, out);
	}
};

#endif
