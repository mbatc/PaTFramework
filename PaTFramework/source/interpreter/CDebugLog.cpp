#include "CDebugLog.h"

#include <stdio.h>
#include <iostream>
#include <ctime>
#include <chrono>

#include "../helpers/MemoryHelper.h"

CDebugLog debug;

using namespace std;

CDebugLog::CDebugLog()
:
    m_pLogFile(0),
    m_pFile(0)
{
	SetLogFile("DEBUG_LOG");
}

CDebugLog::~CDebugLog()
{
    if(m_pFile)
        fclose(m_pFile);

    m_pFile = 0;
}

void CDebugLog::Log(char* msg, bool c_out)
{
    if(!m_pFile)
    {
        m_pFile = fopen(m_pLogFile, "w");
    }
    if(!m_pFile)
    {
        cout << "Could Not Open Log File: " << m_pLogFile << "\n";
    }

    if(m_pFile)
        fprintf(m_pFile,msg);
	if (c_out)
		cout << msg;
}

void CDebugLog::SetLogFile(char* pFilename)
{
    chrono::time_point<std::chrono::system_clock> _now;
    _now = std::chrono::system_clock::now();
    time_t _time = chrono::system_clock::to_time_t(_now);

    char* time =  ctime(&_time);
    time[strlen(time) - 1] = '\0';
    int len = strlen(time) + strlen(pFilename) + 6;

    for(int i = 0; i < strlen(time); i++)
    {
        if(time[i] == ':')
        {
            time[i] = '-';
        }
    }

    MEMORY::SafeDeleteArr(m_pLogFile);
    m_pLogFile = new char[len];
    strcpy(m_pLogFile, pFilename);
    strcpy(&m_pLogFile[strlen(pFilename)], time);
    strcpy(&m_pLogFile[strlen(pFilename) + strlen(time)], ".txt");
}
