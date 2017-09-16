#pragma once

#include "../CTransform.h"
#include "../../helpers/macro.h"
#include "../../MathTransform.h"

#include <vector>

#define derive_ColliderType(cls) derive_crtp(CColliderBase_inherit, cls)

class CCollisionEngine; 
class CColliderBase;

class CCollisionData
{
public:
	CCollisionData() {
		memset(this, 0, sizeof(CCollisionData));
	}

	CVector3		m_collision_point;
	CVector3		m_velocity;
	CColliderBase*	pWith;

	//CPlane		pWith_plane;
};

class CColliderBase 
{
	friend CCollisionEngine;
public:
	CColliderBase();
	~CColliderBase();
	
	unsigned int update();
	
	//Called by the collision engine - checks for a collision between this collider and the collider passed to the 
	//function - if one is found new collision data is added to the collision data list.
	virtual unsigned int		check_collision(CColliderBase* base) = 0;

	CCollisionData				get_collisiondata(int index);
	//Get center point of the collider
	CTransform&					get_transform();
	//Get side closest to this point (use angles/trig) - function will be defined separately for different collider types
	virtual C3DPlane			get_closestside(CVector3 to);

	unsigned int				get_collisioncount();
	unsigned int				add_collision(CColliderBase* b);

	std::string					get_groupname() { return m_group_name; }
protected:
	//Get ur closest side to the colliding object as well as theres to you and find intercept along collision path
	CVector3					get_collision_point(CColliderBase* b);
	std::vector<CCollisionData>	m_data;
private:

	std::string m_group_name;

	CTransform m_transform;
	CTransform m_prev_transform;

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