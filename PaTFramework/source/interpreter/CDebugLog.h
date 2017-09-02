#pragma once
#include <stdio.h>

class CDebugLog
{
public:
    CDebugLog();
    ~CDebugLog();

    void SetLogFile(char* pFilename);
	void Log(char* msg, bool c_out = false);
private:
    char* m_pLogFile;
    FILE* m_pFile;
};

extern CDebugLog debug;
