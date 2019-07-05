/*
 * bstrutil.cpp
 *
 *  Created on: Jul 4, 2019
 *      Author: albert
 */

#include "../include/base/strutil.h"
#include "../include/base/dict.h"
#include "../include/base/dict.h"

#include <string.h>

BString *BStrUtil :: fill_templ(BString *src, bool (*get_val)(const char *key, void *data, BString *out), const char *t_open, const char *t_close, void *data, BString *out)
{
	BString result(src);
	BString insert_buf;

	for (int32 open = 0, close;;)
	{
		// See unit tests for why this has to be so complicated.

		open  = result.find(t_open, open);
		close = result.find(t_close, open + strlen(t_open));

		if (open == -1 || close == -1) break;

		for (int32 next_open; (next_open = result.find(t_open, open + strlen(t_open))) < close && next_open != -1;)	// We only want the inner-most open/close if they happen to be nested.
		{
			open = next_open;
		}

		/* Get string to fill template */
		result.overwrite("", close, 1);	// We overwrite the closer with a null byte so that we can pass the template name as a null terminated string. The closer will be removed anyway so it doesn't matter.

		if (! get_val(result.char_at(open) + strlen(t_open), data, &insert_buf)) return NULL;

		result.remove(open, close - open + strlen(t_close));
		result.insert(&insert_buf, open);

		open += insert_buf.length();	// Don't fill any templates inside what we've just inserted.
		insert_buf.clear();
	}

	if (out)
		return out->own(&result);
	else
		return src->own(&result);
}

static bool get_dict_value(const char *key, BDict *dict, BString *out)
{
	const char *value = (const char *) dict->get(key);

	if (value != NULL)
	{
		out->set(value);
		return true;
	}
	else
	{
		return false;
	}
}

BString *BStrUtil :: fill_templ(BString *src, BDict /* const char * */ *values, const char *t_open, const char *t_close, BString *out)
{
	return BStrUtil::fill_templ<BDict *>(src, &get_dict_value, t_open, t_close, values, out);
}
