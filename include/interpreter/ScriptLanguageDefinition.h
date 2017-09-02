#pragma once

#define MAX_SYMBOL_LEN 3
#define MAX_KEYWORD_LEN 16
#define MAX_TYPE_LEN 16
#define MAX_FUNCTION_LEN 64

#define NSYMBOLS 24
#define NKEYWORDS 9
#define NTYPES 4
#define NFUNCTIONS 5

#define PERSISTANT_VARS "PERSISTANT_VARIABLES"
#define END_MARK '\0'
#define NEW_LINE '\n'
#define END_STATEMENT ';'

#define S_IDENTIFIER "IDENTIFIER"
#define S_SYMBOL "SYMBOL"
#define S_STRING "STRING"
#define S_NUMBER "NUMBER"
#define S_DECIMAL "DECIMAL"
#define S_BOOL "BOOL"
#define S_FUNCTION "FUNCTION"
#define S_TYPE "TYPE"
#define S_KEYWORD "KEYWORD"
#define S_ENDMARK "ENDMARK"

#include <string.h>

const char KEYWORDS[NKEYWORDS][MAX_KEYWORD_LEN] = {
    "if","else","break",
    "do","for","while","do",
    "continue","return"
};

const char TYPES[NTYPES][MAX_TYPE_LEN] = {
    "int",
    "float",
    "bool",
    "string"
};

const char SYMBOLS[NSYMBOLS][MAX_SYMBOL_LEN] = {
    "/","*","+",
    "-","=","+=","-=",
    "*=","/=","!=",
    ">=","<=", "==",
    "(",")","{","}",
    "'","\"","&&","||",
    ";",".",","
};

const char FUNCTIONS[NFUNCTIONS][MAX_FUNCTION_LEN] = {
    "OUTPUT",
    "GetComponent",
    "SaveToFile",
    "ReadFromFile",
    "GetScriptVar"
};

bool IsFunction(char* str);

bool IsSymbol(char* str);

bool IsType(char* str);

bool IsKeyword(char* str);

bool IsBool(char* str);
