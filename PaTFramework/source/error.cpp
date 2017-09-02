#include "error.h"

unsigned int GetErrorID(char* txt)
{
	for (int i = 0; i < sizeof(ERRLIST) / sizeof(SERRORDEF); i++)
	{
		if (ERRLIST[i].m_txt == txt)
			return ERRLIST[i].m_ID;
	}

	return ERROR_UNDEFINED;
}

const char* GetErrorTxt(unsigned int ID)
{
	for (int i = 0; i < sizeof(ERRLIST) / sizeof(SERRORDEF); i++)
	{
		if (ERRLIST[i].m_ID == ID)
			return ERRLIST[i].m_txt.c_str();
	}

	return ERRLIST[0].m_txt.c_str();
}