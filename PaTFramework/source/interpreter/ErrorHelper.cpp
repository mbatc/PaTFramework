#include "ErrorDefinitions.h"
#include "CDebugLog.h"

#include <iostream>
using namespace std;


//#define NO_ERROR 0
//#define ERROR_UNDEFINED_ERROR 1
//#define ERROR_UNDEFINED_OPERATOR 2
//#define ERROR_UNDEFINED_DATA_TYPE 3
//#define ERROR_UNDEFINED_VARIABLE 4
//#define ERROR_OPERATION_NOT_DEFINED 5
//#define ERROR_FILE_NOT_FOUND 6
//#define ERROR_INVALID_VALUE 7
//#define ERROR_REDEFINITION 8
//#define ERROR_UNEXPECTED_TOKEN 9
//#define ERROR_EXPECTED_SYMBOL 10
//#define ERROR_EXPECTED_CLOSING_BRACKET 11

char* GetErrorText(unsigned int err)
{
    switch(err)
    {
    case ERROR_UNDEFINED_ERROR:
        return "ERROR_UNDEFINED_ERROR";
    case ERROR_UNDEFINED_OPERATOR:
        return "ERROR_UNDEFINED_OPERATOR";
    case ERROR_UNDEFINED_VARIABLE:
        return "ERROR_UNDEFINED_VARIABLE";
    case ERROR_UNDEFINED_DATA_TYPE:
        return "ERROR_UNDEFINED_DATA_TYPE";
    case ERROR_OPERATION_NOT_DEFINED:
        return "ERROR_OPERATION_NOT_DEFINED";
    case ERROR_UNEXPECTED_TOKEN:
        return "ERROR_UNEXPECTED_TOKEN";
    case ERROR_REDEFINITION:
        return "ERROR_REDEFINITION";
    case ERROR_EXPECTED_CLOSING_BRACKET:
        return "ERROR_EXPECTED_CLOSING_BRACKET";
    case ERROR_EXPECTED_SYMBOL:
        return "ERROR_EXPECTED_SYMBOL";
    case ERROR_INVALID_VALUE:
        return "ERROR_INVALID_VALUE";
    case ERROR_FILE_NOT_FOUND:
        return "ERROR_FILE_NOT_FOUND";
    case ERROR_INVALID_ARG_COUNT:
        return "ERROR_INVALID_ARG_COUNT";
    case ERROR_INVALID_DATA_TYPE:
        return "ERROR_INVALID_DATA_TYPE";
    case ERROR_NOT_COMPILED:
        return "ERROR_NOT_COMPILED";
    }

    return "NO_ERROR";
}

void OutputErrorMsg(unsigned int ErrorMsg, CToken token)
{
    char msg[512] = {0};
    sprintf(msg, "%s : %s | At col: %d row: %d \n", GetErrorText(ErrorMsg),
            token.GetTokenString(), token.GetCol(), token.GetRow());
    debug.Log(msg, true);
}
