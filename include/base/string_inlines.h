#include <string.h>

inline const char *BString :: set(const char *str)
{
	return this->set(str, strlen(str));
}

inline bool BString :: append(char *str)
{
	return this->append(str, strlen(str));
}

inline bool BString :: append(BString *str)
{
	return this->append(str->string, str->m_size);
}

inline bool BString :: prepend(char *str)
{
	return this->prepend(str, strlen(str));
}

inline bool BString :: prepend(BString *str)
{
	return this->prepend(str->string, str->m_size);
}

inline bool BString :: insert(char *str, int32 offset)
{
	return this->insert(str, strlen(str), offset);
}

inline bool BString :: insert(BString *str, int32 offset)
{
	return this->insert(str->string, str->m_size, offset);
}

inline void BString :: reverse()
{
	this->reverse(this);
}

inline bool BString :: equals(const char *b, size_t size)
{
	return BString::BString :: equals(this, b, size);
}

inline bool BString :: equals(const char *b)
{
	return BString::BString :: equals(this, b);
}

inline bool BString :: equals(BString *b)
{
	return BString::BString :: equals(this, b);
}

inline void BString :: resize(int32 start, int32 end, char fill_char)
{
	this->resize(start, end, this, fill_char);
}

inline const char *BString :: c_str()
{
//	return (const char *) (this->string ? this->string : "");
	return (const char *) this->string;
}
