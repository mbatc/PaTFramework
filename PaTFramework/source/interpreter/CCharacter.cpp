#include "CCharacter.h"

#include "CDebugLog.h"

#include <iostream>
using namespace std;

CCharacter::CCharacter(char cargo, int row, int col, int index, char* src)
:
    m_cargo(cargo),
    m_row(row),
    m_col(col),
    m_index(index),
    m_pSrc(src)
{}

CCharacter::CCharacter()
:
    m_cargo(0),
    m_row(0),
    m_col(0),
    m_index(0),
    m_pSrc(0)
{}

CCharacter::~CCharacter()
{}

char CCharacter::GetCargo()
{
    return m_cargo;
}

char* CCharacter::GetSrc()
{
    return m_pSrc;
}

int CCharacter::GetRow()
{
    return m_row;
}

int CCharacter::GetCol()
{
    return m_col;
}

int CCharacter::GetIndex()
{
    return m_index;
}

void CCharacter::PrintDataToConsole()
{
    char msg[512] = {0};

    switch(m_cargo)
    {
    case '\0':
        sprintf(msg, "%d : %d : %d : END_MARK\n", m_index, m_col, m_row);
        break;
    case '\n':
        sprintf(msg, "%d : %d : %d : NEW_LINE\n", m_index, m_col, m_row);
        break;
    case '\t':
        sprintf(msg, "%d : %d : %d : TAB\n", m_index, m_col, m_row);
        break;
    case ' ':
        sprintf(msg, "%d : %d : %d : SPACE\n", m_index, m_col, m_row);
        break;
    default:
        sprintf(msg, "%d : %d : %d : %c\n", m_index, m_col, m_row ,m_cargo);
        break;
    }

    debug.Log(msg);
}
