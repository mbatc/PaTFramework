#pragma once

#include "../../CStringIDList.h"
#include "../../CLog.h"
#include "../../helpers/CErrorBase.h"

#include <vector>
#include <string>

class CGameObject;

#include "../CComponent.h"

typedef CComponent* (*CREATEFUNCTYPE)(CGameObject*);

class CComponentFactory
{
private:
	CComponentFactory();
	static CComponentFactory* _instance;

public:
	~CComponentFactory();
	static CComponentFactory* get_instance() {
		if (!_instance)
		{
			_instance = new CComponentFactory();
			if (_instance)
				Log(nullptr, DEBUGLOG_LEVEL_INFO, "CComponentFactory instance created (ptr=%p)", _instance);
			else
				Log(nullptr, DEBUGLOG_LEVEL_ERROR, "Failed to create CComponentFactory Instance!");
		}
		return _instance;
	}
	static void destroy_instance() {
		if (_instance)
		{
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CComponentFactory instance destroyed (ptr=%p)", _instance);
			delete _instance;
		}
		_instance = 0;
	}
	
	static const char*	get_component_typename(CComponent* comp);
	static const int	get_component_typeid(CComponent* comp);

	template <class T> static const char*	get_component_typename();
	template <class T> static const int		get_component_typeid();

	template <class T> static const bool	is_type(CComponent* comp);

	CComponent* create_component(std::string name, CGameObject* parent = nullptr, std::string init_string = "");

	template <class T> unsigned int register_component_type(std::string comp_type_name);
	unsigned int assign_uniqueids();

private:

	unsigned int add_component_func(std::string comp_str, CREATEFUNCTYPE func);

	CStringIDList m_createfunc_id;
	//Returns a pointer to the created gameobject - holds the pointer to the template function in the CComponent class
	std::vector<CREATEFUNCTYPE> m_createfunc_list;
};


template<class T> inline const char * CComponentFactory::get_component_typename()
{
	CComponentFactory* cfactory = get_instance();
	T* temp = new T();
	int id = temp->comp_id();
	delete temp;

	return cfactory->m_createfunc_id.get_str(id);
}

template<class T> inline const int CComponentFactory::get_component_typeid()
{
	T* temp = new T(nullptr);
	int id = temp->comp_id();
	delete temp;
	return id;
}

template<class T>
inline const bool CComponentFactory::is_type(CComponent * comp)
{
	if (!comp)
		return false;

	T* temp = new T();
	bool check = (temp->comp_id() == comp->comp_id());
	delete temp;

	return check;
}

template<class T>
inline unsigned int CComponentFactory::register_component_type(std::string comp_type_name)
{
	return add_component_func(comp_type_name, (CREATEFUNCTYPE)T::create);
}
