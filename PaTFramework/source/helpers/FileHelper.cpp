#include "FileHelper.h"
#include "MemoryHelper.h"
#include "StringHelper.h"
#include <string>

char* FILEH::pBuffer = NULL;

int FILEH::GetNextScanLength(FILE * pFile)
{
	fpos_t pos;
	fgetpos(pFile, &pos);

	char temp = '\0';
	bool validCheck = false;
	int bufLen = 0;
	while (temp != ' ' || !validCheck)
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);
			if (temp != ' ' && temp != '\n' && temp != '\r')
			{
				validCheck = true;
				bufLen++;
			}
			if (temp == '\0' || temp == '\n' || temp =='\r')
			{
				temp = ' ';
			}
		}
		else
		{
			break;
		}
	}
	if (validCheck)
		bufLen++;

	fsetpos(pFile, &pos);
	return bufLen;
}

int FILEH::GetStepsToEndOfLine(FILE * pFile)
{
	fpos_t pos;
	fgetpos(pFile, &pos);
	int bufLen = 0;
	bool ValidScan = false;
	char temp = '\0';

	while (!ValidScan || temp != '\n')
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);

			if (temp != ' ')
			{
				ValidScan = true;
			}

			if (ValidScan)
				bufLen++;
		}
		else
		{
			break;
		}
	}
	if (ValidScan)
		bufLen++;
	fsetpos(pFile, &pos);
	return bufLen;
}

char * FILEH::GetFileExtension(char * filename)
{
	if (!filename)
		return NULL;

	int len = strlen(filename);

	for (int i = len; i > 0; i--)
	{
		if (filename[i] == '.')
			return &filename[i];
	}

	return NULL;
}

void FILEH::LOAD::GetStringQuotation( FILE * pFile, char *& dst)
{
	if (!pFile)
		return;

	MEMORY::SafeDeleteArr(dst);
	if (feof(pFile))
		return;
	char temp = '\0';
	char quoteType = '\0';
	int dstLen = 0;

	fread(&temp,sizeof(char),1,pFile);
	while (temp != '"' && temp != '\'')
	{
		if (feof(pFile))
			return;

		fread(&temp, sizeof(char), 1, pFile);
	}
	quoteType = temp;
	do
	{
		if (!feof(pFile))
		{
			fread(&temp, sizeof(char), 1, pFile);
			MEMORY::LIST::AddToList(dst, &temp, dstLen);
		}
	} while (temp != quoteType);

	dst[dstLen - 1] = '\0';
}

void FILEH::LOAD::SafeScanString(FILE * pFile, char *& dst)
{
	if (!pFile)
		return;
	
	MEMORY::SafeDeleteArr(dst);
	dst = new char[GetNextScanLength(pFile)];
	if (!dst)
		return;

	fscanf(pFile, "%s", dst);
}

void FILEH::LOAD::PeekSafeScan(FILE * pFile, char *& dst)
{
	if (!pFile)
		return;

	fpos_t pos;
	fgetpos(pFile, &pos);

	SafeScanString(pFile, dst);

	fsetpos(pFile, &pos);
}

/////////////////////////////////////////////////////////
//NEW FUNCTIONS - SHOULD REPLACE OLD ONES EVENTUALLY
bool FILEH::LOAD::isWhitespace(char c)
{
	return (c == ' ' || c == '\n'
		|| c == '\0' || c == '\t'
		|| c == '\r' || c == '\v' || c == '\f');
}

int FILEH::LOAD::GetNextScanLen(FILE * f)
{
	fpos_t pos;
	fgetpos(f, &pos);
	char c = '\0';

	bool bStartCount = false;

	int len = 0;
	do
	{
		fread(&c, sizeof(char), 1, f);
		switch (bStartCount)
		{
		case true:
			len++;
			break;
		case false:
			if (!isWhitespace(c))
			{
				bStartCount = true;
				len++;
			}
			break;
		}
	} while (!bStartCount || !isWhitespace(c));

	fsetpos(f, &pos);
	return len + 1;
}

char * FILEH::LOAD::ScanLineSafe(FILE * f)
{
	char c = '\0';
	int len = 0;

	MEMORY::SafeDeleteArr(pBuffer);

	do
	{
		fread(&c, sizeof(char), 1, f);
		if (c == '\n')
			c = '\0';

		MEMORY::LIST::AddToList(pBuffer, &c, len);
	} while (c != '\0');
	return pBuffer;
}

char * FILEH::LOAD::ScanQuotationSafe(FILE * f)
{
	MEMORY::SafeDeleteArr(pBuffer);
	char c = '\0';

	do
	{
		fread(&c, sizeof(char), 1, f);
		if (c == '\0')
		{
			pBuffer = NULL;
			return NULL;
		}
	} while (c != '"');

	int len = 0;

	do
	{
		fread(&c, sizeof(char), 1, f);
		if (c == '"')
			c = '\0';
		MEMORY::LIST::AddToList(pBuffer, &c, len);
	} while (c != '\0');

	return pBuffer;
}

char * FILEH::LOAD::ScanSafe(FILE * f)
{
	MEMORY::SafeDeleteArr(pBuffer);
	int l = GetNextScanLen(f);
	if (l < 1)
	{
		pBuffer = NULL;
		return NULL;
	}

	pBuffer = new char[l];

	fscanf(f, "%s", pBuffer);

	return pBuffer;
}
