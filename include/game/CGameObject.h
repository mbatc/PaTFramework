#pragma once

#include "CTransform.h"
#include "CComponent.h"
#include "../CD3DGraphics.h"

#include "collision\CColliderBase.h"

#include <vector>
#include <string>

class CGameObject
{
public:
	CGameObject(std::string name = "(null)");
	~CGameObject();

	void set_parent(CGameObject* parent);
	void add_child(CGameObject* child);
	void rem_child(CGameObject* child);

	CGameObject* get_parent() { return m_parent; }

	virtual unsigned int update();
	virtual unsigned int render(CD3DGraphics& gfx);
	virtual unsigned int startup();
	virtual unsigned int collision(CColliderBase* collider, std::vector<CCollisionData> collisions);

	void set_name(std::string name) { m_name = name; }

	std::string get_name() { return m_name; }
	CTransform& get_transform() { return m_local_transform; }

	CTransform& get_global_transform() { return m_global_transform; }

	unsigned int add_component(CComponent* comp);
	void rem_component(int index);
	void rem_component(std::string name);

	void lerp_to_parent(float factor);

	unsigned int _ID_;
protected:
	float						m_lerp_factor;

	CTransform					m_prev_global_transform;
	CTransform					m_prev_local_transform;

	CTransform					m_global_transform;
	CTransform					m_local_transform;
	std::vector<CComponent*>	m_component;

	std::vector<CGameObject*>	m_child_list;
	CGameObject*				m_parent;

	std::string m_name;
	unsigned int _last_error = 0;
};