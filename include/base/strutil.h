#ifndef __BSTRUTIL_H_
#define __BSTRUTIL_H_

#include <osndef.h>
#include <base/string.h>

class BStrUtil
{
public:
	static BString *escape(const BString *str);
	static BString *unescape(const BString *str);
};

#endif
