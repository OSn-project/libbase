/* Inlines */

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

inline bool BString :: equals(const char *str)
{
	return this->equals(str, strlen(str));
}

inline bool BString :: equals(BString *str)
{
	return this->equals(str->string, str->m_size);
}

inline const char *BString :: c_str()
{
	return (const char *) (this->string ? this->string : "");
}
