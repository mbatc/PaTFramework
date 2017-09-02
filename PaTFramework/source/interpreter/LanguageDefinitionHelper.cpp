#include "ScriptLanguageDefinition.h"

bool IsFunction(char* str)
{
    for(int i = 0; i < NFUNCTIONS; i++)
    {
        if(!strcmp(str,FUNCTIONS[i]))
            return true;
    }
    return false;
}

bool IsSymbol(char* str)
{
    for(int i = 0; i < NSYMBOLS; i++)
    {
        if(!strcmp(str,SYMBOLS[i]))
            return true;
    }
    return false;
}

bool IsType(char* str)
{
    for(int i = 0; i < NTYPES; i++)
    {
        if(!strcmp(str,TYPES[i]))
            return true;
    }
    return false;
}

bool IsKeyword(char* str)
{
    for(int i = 0; i < NKEYWORDS; i++)
    {
        if(!strcmp(str, KEYWORDS[i]))
            return true;
    }
    return false;
}

bool IsBool(char* str)
{
    if(!strcmp(str, "false") || !strcmp(str,"true"))
        return true;

    return false;
}
