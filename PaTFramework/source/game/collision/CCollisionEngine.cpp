#include "CCollisionEngine.h"
#include "../../helpers/MemoryHelper.h"
#include "../../helpers/CErrorBase.h"

#include "CColliderAABB.h"
#include "CColliderSphere.h"

CCollisionEngine* CCollisionEngine::_instance = 0;

unsigned int CCollisionEngine::update()
{
	//****
	//TODO: add collision profiles which will allow colliders to skip detection with specific colliders or groups
	//****

	//Update all colliders (also clears collisions found in the last update)
	for (int i = 0; i < m_group.size(); i++)
	{
		for (int j = 0; j < m_group[i]._list.size(); j++)
		{
			m_group[i]._list[j]._collider->update();
		}
	}

	int result = 0;
	for (int i = 0; i < m_group.size(); i++)
	{
		for (int j = 0; j < m_group[i]._list.size(); j++)
		{
			//Check for collisions within the same group
			for (int k = j + 1; k < m_group[i]._list.size(); k++)
			{
				int temp = check_collision(m_group[i]._list[j]._collider, m_group[i]._list[k]._collider);
				if (temp != 0)
					result = temp;
			}

			//Check for collisions with other groups
			for (int k = i + 1; k < m_group.size(); k++)
			{
				for (int m = 0; m < m_group[k]._list.size(); m++)
				{
					int temp = check_collision(m_group[i]._list[j]._collider, m_group[k]._list[m]._collider);
					if (temp != 0)
						result = temp;
				}
			}
		}
	}

	RET_ERR(result);
}

void CCollisionEngine::destroy_collider(CColliderBase * col)
{
	ColliderGroup g = get_group(col->get_groupname());
	destroy_collider(col, g);
}

void CCollisionEngine::destroy_collider(int id)
{
	for (int i = 0; i < m_group.size(); i++)
	{
		for (int j = 0; j < m_group[i]._list.size(); j++)
		{
			if (id == m_group[i]._list[j]._id)
			{
				destroy_collider(j, m_group[i]);
			}
		}
	}
}

void CCollisionEngine::destroy_collider(CColliderBase * col, ColliderGroup group)
{
	for (int i = 0; i < group._list.size(); i++)
	{
		if (col == group._list[i]._collider)
		{
			destroy_collider(i, group);
		}
	}
}

void CCollisionEngine::destroy_collider(int index, ColliderGroup group)
{
	MEMORY::SafeDelete(group._list[index]._collider);
	unregister_collider(index, group);
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

unsigned int CCollisionEngine::register_collider(CColliderBase * collider, std::string name)
{
	ColliderGroup& g = get_group(name);

	for (int i = 0; i < g._list.size(); i++)
	{
		//Check if the collider is already registered
		if (collider == g._list[i]._collider)
			RET_ERR(1);
	}

	ColliderListItem li;
	li._collider = collider;
	li._id = get_uniqueid();

	g._list.push_back(li);
	return g._list.size() - 1;
}

unsigned int CCollisionEngine::unregister_collider(int index, CCollisionEngine::ColliderGroup g)
{
	if (index < 0 || index >= g._list.size())
		return 1;

	g._list.erase(g._list.begin() + index);
	RET_ERR(0);
}

unsigned int CCollisionEngine::register_group(std::string name)
{
	int id = 1;
	for (int i = 0; i < m_group.size(); i++)
	{
		if (m_group[i]._name == name)
		{
			Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to register collider group '%s': group already exists", name.c_str());
			RET_ERR(1);
		}
		id += m_group[i]._id;
	}

	ColliderGroup g;
	g._id = id;
	g._name = name;
	g._flag = true;

	m_group.push_back(g);
	RET_ERR(0);
}

unsigned int CCollisionEngine::unregister_group(int id)
{
	for (int i = 0; i < m_group.size(); i++)
	{
		if (id == m_group[i]._id)
		{
			for (int j = 0; j < m_group[i]._list.size(); j++)
			{
				//Destroy all colliders in the group before destroy the 
				//group itself
				destroy_collider(j, m_group[i]);
			}
			m_group.erase(m_group.begin() + i);
		}
	}
	return 0;
}

CCollisionEngine::ColliderGroup& CCollisionEngine::get_group(std::string name)
{
	for (int i = 0; i < m_group.size(); i++)
	{
		if (m_group[i]._name == name)
		{
			return m_group[i];
		}
	}

	register_group(name);
	return get_group(name);
}

unsigned int CCollisionEngine::get_uniqueid()
{
	unsigned int id = rand()*MAXUINT32;
	for (int i = 0; i < m_group.size(); i++)
	{
		for (int j = 0; j < m_group[i]._list.size(); j++)
		{
			if (id == m_group[i]._list[i]._id)
				return get_uniqueid();
		}
	}
	return id;
}

void CCollisionEngine::init_collidertype_identifier()
{
	set_colliderid<CColliderAABB>(1);
	set_colliderid<CColliderSphere>(2);
}

void CCollisionEngine::cleanup()
{
	for (int i = 0; i < m_group.size(); i++)
	{
		unregister_group(m_group[i]._id);
	}
}
