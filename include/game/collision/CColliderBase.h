#pragma once

#include "../CTransform.h"
#include "../../helpers/macro.h"
#include "../../MathTransform.h"

#include <vector>

#define derive_ColliderType(cls) derive_crtp(CColliderBase_inherit, cls)

class CCollisionEngine; 
class CColliderBase;
class CGameObject;

class CCollisionData
{
public:
	CCollisionData() {
		memset(this, 0, sizeof(CCollisionData));
	}

	CVector3		m_collision_point;
	CVector3		m_velocity;
	C3DPlane		m_plane;

	CColliderBase*	pWith;
	CVector3		with_velocity;
	CVector3		with_collision_point;
	C3DPlane		with_plane;

	void switch_colliderinfo(CColliderBase* new_with)
	{
		CCollisionData new_data;
		new_data.pWith = new_with;
		new_data.m_velocity = with_velocity;
		new_data.m_collision_point = with_collision_point;
		new_data.m_plane = with_plane;
		new_data.with_collision_point = m_collision_point;
		new_data.with_plane = m_plane;
		new_data.with_velocity = m_velocity;

		*this = new_data;
	}
};

class CColliderBase 
{
	friend CCollisionEngine;
public:
	CColliderBase();
	~CColliderBase();
	
	unsigned int update();
	unsigned int collision();
	
	CCollisionData				get_collisiondata(int index);
	CCollisionData				calculate_collision(CColliderBase* collider);
	//Get center point of the collider
	CTransform&					get_transform();
	//Get side closest to this point (use angles/trig) - function will be defined separately for different collider types
	virtual C3DPlane			get_closestside(CVector3 to);

	unsigned int				get_collisioncount();
	unsigned int				add_collision(CCollisionData data);

	std::string					get_groupname() { return m_group_name; }

	//Get and Set the pointer to the gameobect. The collision() function of the object passed to this function will be
	//envoked when a collision is detected.
	unsigned int				set_gameobject(CGameObject* obj);
	CGameObject*				get_gameobject() { return m_handleObj; }
protected:
	//Get ur closest side to the colliding object as well as theres to you and find intercept along collision path
	virtual CVector3			get_collision_point(CColliderBase* b);
	std::vector<CCollisionData>	m_data;
private:

	std::string m_group_name;

	CTransform m_transform;
	CTransform m_prev_transform;

	CGameObject* m_handleObj;

	virtual int& collider_id() { return _base_id; }
	static int _base_id;
};

template <class T> class CColliderBase_inherit : public CColliderBase
{
	friend CCollisionEngine;
public:
	CColliderBase_inherit()
		:
		CColliderBase() {}

	~CColliderBase_inherit() {}


private:
	int& collider_id() override { return _derived_id; }
	static int _derived_id;
};