#pragma once



class CCharacter
{
public:
    CCharacter();
    CCharacter(char cargo, int row, int col, int index, char* src);
    ~CCharacter();

    char GetCargo();
    char* GetSrc();
    int GetRow();
    int GetCol();
    int GetIndex();

    void PrintDataToConsole();
private:
    char m_cargo;
    int m_col, m_row;
    int m_index;

    char* m_pSrc;
};
