#include "CComponentFactory.h"
#include "../../helpers/MemoryHelper.h"
CComponentFactory* CComponentFactory::_instance = nullptr;

CComponentFactory::CComponentFactory()
{}

CComponentFactory::~CComponentFactory()
{}

const char * CComponentFactory::get_component_typename(CComponent * comp)
{
	CComponentFactory* cfactory = get_instance();
	return cfactory->m_createfunc_id.get_str(comp->comp_id());
}

const int CComponentFactory::get_component_typeid(CComponent * comp)
{
	return comp->comp_id();
}

CComponent * CComponentFactory::create_component(std::string name, CGameObject * parent, std::string init_string)
{
	int i = m_createfunc_id.get_id(name.c_str());

	if (i < 0 || i >= m_createfunc_list.size())
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "No component create function registered with strid '%s'", name.c_str());
		return nullptr;
	}

	CComponent* ret = m_createfunc_list[i](parent);

	if (!ret)
		Log(this, DEBUGLOG_LEVEL_WARNING, "Call to '%s'::create(CGameObject* parent) [ptr=%p] Failed", name.c_str(), m_createfunc_list[i]);

	//Some initialisation required for underlying CScriptCOMObject to function properlly
	ret->COMSetType(ret->comp_id());

	if (init_string.length() > 0)
		ret->COMInit(init_string);
	else
		ret->COMInit();
	return ret;
}

unsigned int CComponentFactory::assign_uniqueids()
{
	unsigned int ret = 0;
	for (int i = 0; i < m_createfunc_id.count(); i++)
	{
		const int comp_id = m_createfunc_id.get_id(i);
		const char* comp_str = m_createfunc_id.get_str(comp_id);

		CComponent* temp_component = create_component(comp_str);
		if (!temp_component)
		{
			Log(this, DEBUGLOG_LEVEL_WARNING, "Failed to set Component ID (%d) for ID-String '%s' - could not create component", 
				comp_id,
				comp_str);
			ret++;
			continue;
		}

		temp_component->comp_id() = comp_id;
		MEMORY::SafeDelete(temp_component);
	}
	return ret;
}

unsigned int CComponentFactory::add_component_func(std::string comp_str,CREATEFUNCTYPE func)
{
	m_createfunc_list.push_back(func);
	if (m_createfunc_id.add_pair(comp_str.c_str(), (unsigned int)(m_createfunc_list.size() - 1)))
		return 1;
	return 0;
}

