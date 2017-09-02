#pragma once

//This will be properlly filled out once the interpreter code has been merged with this project
//Right now its just here so i can put down some ideas related to scripting

#include <string>

struct VARIABLE_IDENTITY
{
	char* VAR_NAME;
	void* VARIABLE;
	unsigned int TYPE;
};

class CScriptCOMRegistry;

class CScriptCOMObject
{
	friend CScriptCOMRegistry;
public:

	CScriptCOMObject(unsigned long id = 0x00);
	~CScriptCOMObject();

	unsigned int	COMGetID() const;
	unsigned int	COMGetType() const;

	void			COMSetID(unsigned int id);

	void			COMSetParent(void* parent);
	const void*		COMGetParent();

	char*			COMGenerateInitString();
	virtual char*	COMSaveVariable(VARIABLE_IDENTITY i) { return 0; }

	//THIS FUNCTION CAN ONLY BE USED IF THE VARIABLE TYPE IS KNOWN
	void			COMSetVariable(char* NAME, void* data, unsigned int TYPE);
	void			COMSetVariable(char* NAME, char* str_var);

	VARIABLE_IDENTITY*	COMGetVarID(char* NAME);
	VARIABLE_IDENTITY*	COMGetVarID(int index);
	VARIABLE_IDENTITY*	COMGetVarID(void* var);
	int					COMGetVarCount();
protected:
	//This function is overidden by the parent object to Initialise the variable identities
	//NOTE - if a string is registered it must be a String object (see String.h)
	virtual void	COMInitVariables() {};
	void			COMAddVariableIdentity(void* VAR, char* NAME, unsigned int TYPE);

	void*			COMGetVariable(char* NAME);
	unsigned int	COMGetVariableType(char* NAME);
	
	void			COMSetType(unsigned int id);

	template <class T> void	COMSetVariable(char* NAME, T data);

	//ONLY PASS A POINTER OF A TYPE DERIVED FROM THIS CLASS
	template <class T> void COMGetPointer(T*& assignTo);

	void				COMInit(std::string init_str) { COMInit(); COMInitFromString(init_str); }
	void				COMInit() { COMInitVariables(); COM_bInitialised = true; }
private:

	void				COMInitFromString(std::string str);
	int					COMGetVarValuePlaceInStr(std::string str, std::string var_name);

	void				COMClear();
	
	unsigned int		COM_objectID;
	unsigned int		COM_objectType;

	VARIABLE_IDENTITY*	COM_pVarID;
	int					COM_nVarID;

	//Purely for identification
	void*				COM_pParent;

	bool				COM_bInitialised;
};

template<class T>
inline void CScriptCOMObject::COMSetVariable(char * NAME, T data)
{
	*(T*)COMGetVariable(NAME) = data;
}

/*
This function is only safe to use in a specific case
BEFORE CALLING THE FUNCTION CHECK THAT BOTH ARE THE SAME DERIVED CLASS
----------------------------------------------------------------------
CScriptCOMObject* obj = new (DERIVED_CLASS)()

(DERIVED_CLASS)* derived = new (DERIVED_CLASS)()

//This WONT work
derived = obj

//This WILL work
obj->COMGetPointer(derived)
-----------------------------------------------------------------------
*/
template<class T>
inline void CScriptCOMObject::COMGetPointer(T *& assignTo)
{
	assignTo = (T*)this;
}
