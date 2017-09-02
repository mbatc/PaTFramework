#include "CToken.h"
#include "../helpers/MemoryHelper.h"

#include "CDebugLog.h"

#include <iostream>
#include <stdio.h>

using namespace std;

CToken::CToken(int index)
:
    m_pType(0),
    m_pCharacter(0),
    m_nCharacters(0),
    m_index(index),
    m_row(0),
    m_col(0),
    m_pCargo(0)
{}

CToken::CToken()
:
    m_pType(0),
    m_pCharacter(0),
    m_nCharacters(0),
    m_index(0),
    m_row(0),
    m_col(0),
    m_pCargo(0)
{}

CToken::~CToken()
{
    //MEMORY::SafeDeleteArr(m_pCharacter);
    //MEMORY::SafeDeleteArr(m_pCargo);
}

void CToken::SetType(char* type)
{
    MEMORY::SafeDeleteArr(m_pType);
    m_pType = new char[strlen(type) + 1];
    strcpy(m_pType, type);
}

char* CToken::GetTokenString()
{
    if(!m_pCargo)
    {
        char* temp = new char[m_nCharacters + 1];
        for(int i = 0; i < m_nCharacters; i++)
        {
            temp[i] = m_pCharacter[i].GetCargo();
        }
        temp[m_nCharacters] = '\0';
        m_pCargo = temp;
    }
    return m_pCargo;
}

CCharacter* CToken::GetTokenData()
{
    return m_pCharacter;
}

char* CToken::GetType()
{
    return m_pType;
}

void CToken::AddCharacter(CCharacter newChar)
{
    if(m_nCharacters == 0)
    {
        m_row = newChar.GetRow();
        m_col = newChar.GetCol();
    }

    MEMORY::LIST::AddToList(m_pCharacter, &newChar, m_nCharacters);
}

int CToken::GetRow()
{
    return m_row;
}

int CToken::GetCol()
{
   return m_col;
}

void CToken::PrintDataToConsole()
{
    char msg[512] = {0};
    if(GetTokenString()[0] == '\0')
    {
        sprintf(msg, "%d : %d : %d : %s : END_MARK\n", m_index, m_row, m_col, m_pType);
    }
    else if(GetTokenString()[0] == '\n')
    {
        sprintf(msg, "%d : %d : %d : %s : NEW_LINE\n", m_index, m_row, m_col, m_pType);
    }
    else if(GetTokenString()[0] == '\t')
    {
        sprintf(msg, "%d : %d : %d : %s : TAB\n", m_index, m_row, m_col, m_pType);
    }
    else
    {
        sprintf(msg, "%d : %d : %d : %s : %s\n", m_index, m_row, m_col, m_pType, m_pCargo);
    }

    debug.Log(msg);
}
