#include <stdio.h>
#include <cstring>

#include "StringHelper.h"
#include "MemoryHelper.h"

char* STRING::Copy(char*& dest, const char* src)
{
	MEMORY::SafeDeleteArr(dest);
	int len = strlen(src);

	if(len == 0)
		return NULL;

	dest = new char[len + 2];
	strcpy_s(dest,sizeof(char)*(len+2),src);

	return dest;
}

void STRING::Append(char*& dest, const char* first, const char* second)
{
	char* t_first = NULL;
	char* t_second = NULL;
	int firstLen = 0;
	int secondLen = 0;

	if (first)
	{
		STRING::Copy(t_first, first);
		firstLen = strlen(first);
	}
	if (second)
	{
		STRING::Copy(t_second, second);
		secondLen = strlen(second);
	}
	
	MEMORY::SafeDeleteArr(dest);

	if(firstLen + secondLen == 0)
		return;

	dest = new char[firstLen + secondLen + 1];
	for(int i = 0; i < firstLen; i++)
	{
		dest[i] = t_first[i];
	}
	for(int i = 0; i < secondLen; i++)
	{
		dest[i + firstLen] = t_second[i];
	}
	dest[firstLen + secondLen] = '\0';
}

char ** STRING::Split(const char * str, int & nStrings)
{
	nStrings = 0;
	char** split = 0;
	char* cpy = 0;
	char* string = 0;
	STRING::Copy(cpy, str);
	char* cpy_s = cpy;

	do
	{
		string = 0;
		if (!cpy)
			return NULL;	
		while (cpy[0] == ' ' || cpy[0] == '\n' || cpy[0] == '\r')
		{
			cpy = &cpy[1];
		}

		if (cpy[0] == '\0')
			return split;

		char* space = 0;
		char* ret = 0;
		char* endl = 0;
		if (cpy[0] == '"' || cpy[0] == '\'')
		{

			STRING::ScanQuotations(string, cpy);
		}
		else
		{

			space = ScanFromStart(cpy, " ");
			ret = ScanFromStart(cpy, "\r");
			endl = ScanFromStart(cpy, "\n");

			if (!space && ret != 0)
				space = ret;
			if (!space && endl != 0)
				space = endl;

			if (ret < space && ret != 0)
				space = ret;
			if (endl < space && endl != 0)
				space = endl;

			if (!space)
			{
				STRING::Copy(string, cpy);
				MEMORY::LIST::AddToListArr(split, string, nStrings);
				break;
			}

			*space = '\0';
			STRING::Copy(string, cpy);
		}
		MEMORY::LIST::AddToListArr(split, string, nStrings);
		cpy = &cpy[strlen(string) + 1];
		if (!space)
			cpy = &cpy[2];
	} while (true);

	MEMORY::SafeDeleteArr(cpy_s);
	return split;
}

void STRING::ScanQuotations(char *& dst, char * src)
{
	if (!src)
		return;

	MEMORY::SafeDeleteArr(dst);

	char temp = '\0';
	char quoteType = '\0';
	int dstLen = 0;

	temp = src[0];
	while (src[0] != '"' && src[0] != '\'')
	{
		if (*src == '\0')
			return;
		src = &src[1];
	}
	quoteType = src[0];
	src = &src[1];
	do
	{
		if (src[0] != '\0')
		{

			MEMORY::LIST::AddToList(dst, &src[0], dstLen);
			src = &src[1];
		}
	} while (src[0] != quoteType);

	temp = '\0';
	MEMORY::LIST::AddToList(dst,&temp , dstLen);
}

char * STRING::ScanFromEnd(char * src, char * match)
{
	int src_len = strlen(src);
	int match_len = strlen(match);

	char* temp = NULL;
	STRING::Copy(temp, src);

	for (int i = src_len; i >= 0; i--)
	{
		if (src[i] == match[0])
		{
			if (src_len - i < match_len)
				continue;

			if (strstr(&src[i], match) == &src[i])
				return &src[i];
		}
	}

	return nullptr;
}

char * STRING::ScanFromStart(char * src, char * match)
{
	return strstr(src, match);
}
