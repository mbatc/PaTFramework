#pragma once

#include "CScriptCOMObject.h"
#include "CLog.h"

class CScriptCOMRegistry
{
private:
	CScriptCOMRegistry();
	static CScriptCOMRegistry* _instance;

public:
	static CScriptCOMRegistry* get_instance() {
		if (!_instance)
		{
			_instance = new CScriptCOMRegistry;
			if (_instance)
				Log(nullptr, DEBUGLOG_LEVEL_INFO, "CScriptCOMRegistry instance created (ptr=%p)", _instance);
			else
				Log(nullptr, DEBUGLOG_LEVEL_FATAL, "CScriptCOMRegistry could not be created");
		}
		return _instance;
	}

	unsigned int RegisterCOMObject(CScriptCOMObject* p);
	bool Remove(CScriptCOMObject* p);
	CScriptCOMObject* GetCOMObject(unsigned int ID, void* parent = 0);

	void Clear();
private:
	unsigned int GenerateID();

	CScriptCOMObject**	m_pCOMObj;
	int					m_nCOMObj;
};