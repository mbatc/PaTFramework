#include "CCollisionEngine.h"
#include "../../helpers/MemoryHelper.h"
#include "../../helpers/CErrorBase.h"

#include "CColliderAABB.h"
#include "CColliderSphere.h"

unsigned int CCollisionEngine::update()
{
	int result = 0;
	for (int i = 0; i < m_collider.size(); i++)
	{
		for (int j = i+1; j < m_collider.size(); j++)
		{
			check_collision(m_collider[i]._collider,m_collider[j]._collider);
		}
	}

	RET_ERR(result);
}

void CCollisionEngine::destroy_collider(CColliderBase * col)
{
	for (int i = 0; i < m_collider.size(); i++)
	{
		if (col == m_collider[i]._collider)
		{
			MEMORY::SafeDelete(m_collider[i]._collider);
			unregister_collider(i);
		}
	}
}

void CCollisionEngine::destroy_collider(int id)
{
	for (int i = 0; i < m_collider.size(); i++)
	{
		if (id == m_collider[i]._id)
		{
			MEMORY::SafeDelete(m_collider[i]._collider);
			unregister_collider(i);
		}
	}
}

unsigned int CCollisionEngine::check_collision(CColliderBase * col, CColliderBase* col2)
{
	bool bCollision = false;

	if (CCollisionEngine::is_type<CColliderAABB>(col))
	{
		CColliderAABB* a = (CColliderAABB*)col;
		if (CCollisionEngine::is_type <CColliderAABB>(col2))
		{
			CColliderAABB* b = (CColliderAABB*)col2;
			bCollision = Collision_AABBCheck(a->get_boundmin(), a->get_boundmax(),
				b->get_boundmin(), b->get_boundmax());
		}
		else if (CCollisionEngine::is_type<CColliderSphere>(col2))
		{
			CColliderSphere* b = (CColliderSphere*)col2;
			bCollision = Collision_SphereAABBCheck(b->get_transform().get_translation(),
				b->get_radius(), a->get_boundmin(), a->get_boundmax());
		}
	}
	else if (CCollisionEngine::is_type<CColliderSphere>(col))
	{
		CColliderSphere* a = (CColliderSphere*)col;
		if (CCollisionEngine::is_type <CColliderAABB>(col2))
		{
			CColliderAABB* b = (CColliderAABB*)col2;
			bCollision = Collision_SphereAABBCheck(a->get_transform().get_translation(),
				a->get_radius(), b->get_boundmin(), b->get_boundmax());
		}
		else if (CCollisionEngine::is_type<CColliderSphere>(col2))
		{
			CColliderSphere* b = (CColliderSphere*)col2;
			bCollision = Collision_SphereCheck(b->get_transform().get_translation(),
				b->get_radius() + a->get_radius(), a->get_transform().get_translation());
		}
	}

	if (bCollision)
	{
		col->add_collision(col2);
		col2->add_collision(col);
	}
	return 0;
}

unsigned int CCollisionEngine::register_collider(CColliderBase * collider)
{
	for (int i = 0; i < m_collider.size(); i++)
	{
		//Check if the collider is already registered
		if (collider = m_collider[i]._collider)
			return 1;
	}

	ColliderListItem li;
	li._collider = collider;
	li._id = get_uniqueid();

	m_collider.push_back(li);
	return 0;
}

unsigned int CCollisionEngine::unregister_collider(int index)
{
	if (index < 0 || index >= m_collider.size())
		return 1;

	m_collider.erase(m_collider.begin() + index);
	return 0;
}

unsigned int CCollisionEngine::get_uniqueid()
{
	unsigned int id = rand()*MAXUINT32;
	for (int i = 0; i < m_collider.size(); i++)
	{
		if (id == m_collider[i]._id)
			return get_uniqueid();
	}
	return id;
}
