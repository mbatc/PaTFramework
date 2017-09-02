#pragma once
#include <cstdio>

namespace FILEH
{
	int GetNextScanLength(FILE* pFile);
	int GetStepsToEndOfLine(FILE* pFile);
	char* GetFileExtension(char* filename);

	namespace LOAD
	{
		void GetStringQuotation(FILE* pFile, char*& dst);
		void SafeScanString(FILE* pFile, char*& dst);
		void PeekSafeScan(FILE* pFile, char*&dst);

		bool isWhitespace(char c);

		int GetNextScanLen(FILE* f);
		char* ScanLineSafe(FILE* f);
		char* ScanQuotationSafe(FILE* f);
		char* ScanSafe(FILE* f);

	}
	extern char* pBuffer;
}