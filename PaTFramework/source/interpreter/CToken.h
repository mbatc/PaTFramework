#pragma once

#include "CCharacter.h"
#include "ScriptLanguageDefinition.h"

class CToken
{
public:
    CToken();
    CToken(int index);
    ~CToken();

    void AddCharacter(CCharacter newChar);
    void SetType(char* TYPE);

    CCharacter* GetTokenData();
    char* GetTokenString();
    char* GetType();

    int GetCol();
    int GetRow();

    void PrintDataToConsole();

private:
    CCharacter* m_pCharacter;
    int m_nCharacters;

    int m_row, m_col, m_index;

    char* m_pType;
    char* m_pCargo;
};
