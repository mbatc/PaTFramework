#pragma once

#include <string>
#include "../CScriptCOMObject.h"

#include "../interpreter/DataTypeDefinitions.h"
#include "../helpers/macro.h"

#include "collision\CColliderBase.h"

#define derive_gamecomponent(clsname) derive_crtp(CComponentBase, clsname)

class CGameObject;
class CD3DGraphics;

class CComponentFactory;

class CComponent : public CScriptCOMObject
{
	friend CComponentFactory;

public:
	CComponent(CGameObject* parent);
	~CComponent() {}
	
	virtual unsigned int startup() { return 0; }
	virtual unsigned int update() { return 0; }
	virtual unsigned int render(CD3DGraphics& gfx) { return 0; }
	virtual unsigned int collision(CColliderBase* object_collider, CCollisionData data, std::string with_group) { return 0; }

	void handle_collisions(bool b) { m_bHandleCollision = b; }
	bool handle_collisions() { return m_bHandleCollision; }
	bool handle_collisions(std::string group_name);
	bool handle_collisions(CColliderBase* base);

	void add_collisionprofile(std::string group_name);
	void add_collisionprofile(CColliderBase* base);
	void rem_collisionprofile(std::string group_name);
	void rem_collisionprofile(CColliderBase* base);

	void set_parent(CGameObject* parent) { m_pParent = parent; COMSetParent(parent); }
	void set_name(std::string name) { m_name = name; }
	std::string get_name() { return m_name; }
protected:
	CGameObject*	m_pParent;
	std::string		m_name;

private:
	virtual int& comp_id() { return _base_id; }
	static int _base_id;

	bool m_bHandleCollision;

	std::vector<std::string> m_active_collider_group;
	std::vector<CColliderBase*> m_active_collider;
};



//Add another layer of abstraction so that the comp_id() function 
//code doesnt need to be included in every derived class
//as well as the _derived_id variable

//This is needed so that all components can be stored in a single CComponent list
template <class T> class CComponentBase : public CComponent
{
	friend CComponentFactory;
public:
	CComponentBase(CGameObject* obj)
		:
		CComponent(obj){}

	~CComponentBase(){}
private:
	int& comp_id() override { return _derived_id; }
	static int _derived_id;

	static T* create(CGameObject* parent) { return new T(parent); }
};