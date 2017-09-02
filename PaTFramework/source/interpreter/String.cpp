#include "String.h"
#include "../helpers/MemoryHelper.h"

#include <string.h>
#include <stdlib.h>

String::String()
:
    m_pStr(0)
{
	SetString("");
}

String::String(const char * in)
	:
	m_pStr(0)
{
	SetString(in);
}

String::~String()
{

}

void String::SetString(const char* str)
{
    MEMORY::SafeDeleteArr(m_pStr);
    m_pStr = new char[strlen(str) + 1];
    strcpy(m_pStr, str);
}

char* String::GetString()
{
    return m_pStr;
}

int String::GetStringLen()
{
    if(m_pStr)
        return strlen(m_pStr);

    return 0;
}

int String::toInt()
{
	if (m_pStr)
		return atoi(m_pStr);

	return 0;
}

float String::toFloat()
{
	if (m_pStr)
		return atof(m_pStr);
	return 0.0f;
}

bool String::toBool()
{
	return (!strcmp(m_pStr, "true")) ||
		(!strcmp(m_pStr, "TRUE")) ||
		(!strcmp(m_pStr, "1"));
}
