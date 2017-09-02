#include "CScriptCOMRegistry.h"
#include "helpers/MemoryHelper.h"

#include <iostream>

using namespace std;

CScriptCOMRegistry* CScriptCOMRegistry::_instance;

CScriptCOMRegistry::CScriptCOMRegistry()
	:
	m_pCOMObj(0),
	m_nCOMObj(0)
{}

unsigned int CScriptCOMRegistry::RegisterCOMObject(CScriptCOMObject * p)
{
	if (!p)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "CScriptCOMRegistery: ERROR - cannot register NULL object...\n");
		return false;
	}

	p->COMSetID(GenerateID());

	MEMORY::LIST::AddToListArr(m_pCOMObj, p, m_nCOMObj);
}

bool CScriptCOMRegistry::Remove(CScriptCOMObject * p)
{
	for (int i = 0; i < m_nCOMObj; i++)
	{
		if (m_pCOMObj[i] == p)
		{
			MEMORY::LIST::RemoveItem(m_pCOMObj, m_nCOMObj, i);
			return true;
		}
	}
	Log(this,DEBUGLOG_LEVEL_WARNING, "CScriptCOMRegistry: ERROR - Unable to remove CScriptCOMObject (is not registered) [%p]\n", p);

	return false;
}


CScriptCOMObject* CScriptCOMRegistry::GetCOMObject(unsigned int id, void* parent)
{
	for (int i = 0; i < m_nCOMObj; i++)
	{
		if (m_pCOMObj[i]->COMGetID() == id)
		{
			if(m_pCOMObj[i]->COMGetParent() == parent)
				return m_pCOMObj[i];
		}
	}
	Log(this, DEBUGLOG_LEVEL_WARNING,"CScriptCOMRegistry: Unable to retrieve CScriptCOMObject\n");

	if (!parent)
		Log(this, DEBUGLOG_LEVEL_WARNING,"\tNo root object with the ID(%d) has been registered\n",id);
	else
		Log(this, DEBUGLOG_LEVEL_WARNING,"\t: Parent object at (%p) has no child registered with the ID(%d)\n", parent,id);
	return nullptr;
}

void CScriptCOMRegistry::Clear()
{
	MEMORY::SafeDeleteArr(m_pCOMObj);
	m_nCOMObj = 0;
}

unsigned int CScriptCOMRegistry::GenerateID()
{
	unsigned int id = (rand()*UINT_MAX);
	for (int i = 0; i < m_nCOMObj; i++)
	{
		if (id == m_pCOMObj[i]->COMGetID())
			id = GenerateID();
	}

	return id;
}
