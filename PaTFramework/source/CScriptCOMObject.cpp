#include "CScriptCOMObject.h"
#include "helpers\MemoryHelper.h"
#include "helpers\StringHelper.h"
//#include "helpers\SystemHelper.h"

#include <iostream>
using namespace std;


#include "interpreter\OperatorFunctions.h"
#include "interpreter\String.h"

#include "CScriptCOMRegistry.h"

CScriptCOMObject::CScriptCOMObject(unsigned long id)
	:
	COM_pVarID(nullptr),
	COM_nVarID(0),
	COM_objectID(id),
	COM_pParent(nullptr),
	COM_bInitialised(false),
	COM_objectType(0)
{
	CScriptCOMRegistry::get_instance()->RegisterCOMObject(this);
}

CScriptCOMObject::~CScriptCOMObject()
{
	COMClear();
	CScriptCOMRegistry::get_instance()->Remove(this);
}

void CScriptCOMObject::COMInitFromString(std::string str)
{
	for (int i = 0; i < COMGetVarCount(); i++)
	{
		int value_index = COMGetVarValuePlaceInStr(str, COM_pVarID[i].VAR_NAME);
		if (value_index < 0)
			break;
		String value_string;
		if (str[value_index] == '"' || str[value_index] == '\'')
		{
			for (int j = 1; str[value_index] != str[value_index + j] && i < str.length(); i++);
				value_string += str[value_index + i];
		}
		else
		{
			for (int j = 0; str[value_index + j] != ' ' && str[value_index + j] != '\t' &&
				str[value_index + j] != '\n' && str[value_index + j] != '\0' ; j++)
				value_string += str[value_index + j];
		}

		COMSetVariable(COM_pVarID[i].VAR_NAME, &value_string, SCRIPT_VAR_TYPE::SSTR);
	}
}

int CScriptCOMObject::COMGetVarValuePlaceInStr(std::string str, std::string var_name)
{
	int location = -1;
	for (int i = 0; i < str.length(); i++)
	{
		i = str.find(var_name + std::string(":"), 0);
		if (i == std::string::npos)
			return -1;

		char last_quote = '\0';
		for (int j = 0; j < i; j++)
		{
			if (str[j] == '"' || str[j] == '\'')
			{
				if (last_quote == '\0')
					last_quote = str[j];
				else if (last_quote == str[j])
					last_quote = '\0';
			}
		}

		if (last_quote)
			continue;
		
		i++;
		for (; i < str.length(); i++)
			if (str[i-1] == ':')
				break;

		location = str.find_first_not_of(" \t",i);
		break;
	}
	
	return location;
}

void CScriptCOMObject::COMClear()
{
	for (int i = 0; i < COM_nVarID; i++)
		MEMORY::SafeDeleteArr(COM_pVarID[i].VAR_NAME);
	MEMORY::SafeDeleteArr(COM_pVarID);
	COM_nVarID = 0;
}

void CScriptCOMObject::COMAddVariableIdentity(void * VAR, char * NAME, unsigned int TYPE)
{
	VARIABLE_IDENTITY var_id;
	var_id.VAR_NAME = 0;
	var_id.TYPE = TYPE;
	var_id.VARIABLE = VAR;
	STRING::Copy(var_id.VAR_NAME, NAME);
	for (int i = 0; i < COM_nVarID; i++)
	{
		if (!strcmp(NAME, COM_pVarID[i].VAR_NAME))
		{
			Log(this, DEBUGLOG_LEVEL_WARNING, "CScriptCOMObject: COMAddVariableIdentity() - Definition for '%s' already exists\n",NAME);
		}
	}
	MEMORY::LIST::AddToList(COM_pVarID, &var_id, COM_nVarID);
}

void * CScriptCOMObject::COMGetVariable(char * NAME)
{
	for (int i = 0; i < COM_nVarID; i++)
	{
		if (!strcmp(NAME, COM_pVarID[i].VAR_NAME))
		{
			return COM_pVarID[i].VARIABLE;
		}
	}
	Log(this, DEBUGLOG_LEVEL_INFO,"CScriptCOMObect: Unregistered Variable - %s check COMInitVariables() for class type of (ptr=%p)"
		,NAME,this);

	return nullptr;
}

unsigned int CScriptCOMObject::COMGetVariableType(char * NAME)
{
	for (int i = 0; i < COM_nVarID; i++)
	{
		if (!strcmp(NAME, COM_pVarID[i].VAR_NAME))
		{
			return COM_pVarID[i].TYPE;
		}
	}
	return SCRIPT_VAR_TYPE::SUNKNOWN;
}

void CScriptCOMObject::COMSetVariable(char * NAME, void * data, unsigned int TYPE)
{
	SCRIPT::OPERATOR::assignVar(COMGetVariable(NAME), COMGetVariableType(NAME), data, TYPE);
}

void CScriptCOMObject::COMSetVariable(char * NAME, char * str_var)
{
	if (!str_var)
		return;

	String s;
	s = str_var;

	void * var = COMGetVariable(NAME);
	unsigned int type = COMGetVariableType(NAME);

	if (!var)
		return;
	if (!type == SCRIPT_VAR_TYPE::SUNKNOWN)
		return;

	//Special case if the type is an object
	if (type == SCRIPT_VAR_TYPE::SOBJECT)
	{
		CScriptCOMObject* obj = 0;/*SYSTEM::GetScriptCOMObj(s.toInt(),COM_pParent);*/
		if (!obj)
			return;		
		SCRIPT::OPERATOR::assignVar(var, type, &obj, SCRIPT_VAR_TYPE::SOBJECT);
	}
	else
	{
		SCRIPT::OPERATOR::assignVar(var, type, &s, SCRIPT_VAR_TYPE::SSTR);
	}
}

VARIABLE_IDENTITY * CScriptCOMObject::COMGetVarID(char * NAME)
{
	if (!NAME)
		return nullptr;
	for (int i = 0; i < COM_nVarID; i++)
	{
		if (!strcmp(NAME,COM_pVarID[i].VAR_NAME))
			return &COM_pVarID[i];
	}
	return nullptr;
}

VARIABLE_IDENTITY * CScriptCOMObject::COMGetVarID(int index)
{
	if (index < 0 || index >= COM_nVarID)
		return nullptr;
	return &COM_pVarID[index];
}

VARIABLE_IDENTITY * CScriptCOMObject::COMGetVarID(void * var)
{
	for (int i = 0; i < COM_nVarID; i++)
	{
		if (var == COM_pVarID[i].VARIABLE)
			return &COM_pVarID[i];
	}
	return nullptr;
}

int CScriptCOMObject::COMGetVarCount()
{
	return COM_nVarID;
}

void CScriptCOMObject::COMSetID(unsigned int id)
{
	COM_objectID = id;
}

void CScriptCOMObject::COMSetParent(void * parent)
{
	if (!COM_pParent)
	{
		Log(this, DEBUGLOG_LEVEL_INFO,"CScriptCOMObject (%d): Parent set to: %p", COM_objectID, parent);
	}
	else
	{
		Log(this, DEBUGLOG_LEVEL_INFO,"CScriptCOMObject (%d): Parent updated from (%p) to: %p", COM_objectID, COM_pParent, parent);
	}
	COM_pParent = parent;
}

const void * CScriptCOMObject::COMGetParent()
{
	return COM_pParent;
}

char * CScriptCOMObject::COMGenerateInitString()
{
	char* str = 0;
	STRING::Copy(str,"");

	for (int i = 0; i < COM_nVarID; i++)
	{
		char* name = COM_pVarID[i].VAR_NAME;
		void* var = COM_pVarID[i].VARIABLE;
		unsigned int type = COM_pVarID[i].TYPE;
		String val;

		char* custom = COMSaveVariable(COM_pVarID[i]);
		if (custom)
		{
			STRING::Append(str,str,custom);
			continue;
		}

		if (type == SCRIPT_VAR_TYPE::SOBJECT)
		{
			var = new int;
			*(int*)var = (*(CScriptCOMObject**)COM_pVarID[i].VARIABLE)->COMGetID();
			type = SCRIPT_VAR_TYPE::SINT;
		}

		STRING::Append(str, str, COM_pVarID[i].VAR_NAME);
		STRING::Append(str, str, ": ");

		SCRIPT::OPERATOR::assignVar(&val, SCRIPT_VAR_TYPE::SSTR, var, type);
		
		if (type == SCRIPT_VAR_TYPE::SSTR)
			STRING::Append(str, str, "\"");
		STRING::Append(str, str, val.GetString());
		if (type == SCRIPT_VAR_TYPE::SSTR)
			STRING::Append(str, str, "\"");
		STRING::Append(str, str, " ");
	}

	return str;
}

void CScriptCOMObject::COMSetType(unsigned int id)
{
	COM_objectType = id;
}

unsigned int CScriptCOMObject::COMGetID() const
{
	return COM_objectID;
}

unsigned int CScriptCOMObject::COMGetType() const
{
	return COM_objectType;
}
