#include "CComponent.h"
#include "components\CComponentFactory.h"

int CComponent::_base_id;

CComponent::CComponent(CGameObject* parent) : m_pParent(parent), 
	CScriptCOMObject()
{
	_base_id = -1;
}

bool CComponent::handle_collisions(std::string group_name)
{
	if (m_active_collider_group.size() == 0 && m_active_collider.size() == 0 && m_bHandleCollision)
		return true;

	for (int i = 0; i < m_active_collider_group.size(); i++)
	{
		if (m_active_collider_group[i] == group_name)
			return true;
	}

	return false;
}

bool CComponent::handle_collisions(CColliderBase * base)
{
	if (m_active_collider_group.size() == 0 && m_active_collider.size() == 0 && m_bHandleCollision)
		return true;
	
	if (handle_collisions(base->get_groupname()))
		return true;

	for (int i = 0; i < m_active_collider.size(); i++)
	{
		if (m_active_collider[i] == base)
			return true;
	}

	return false;
}

void CComponent::add_collisionprofile(std::string group_name)
{
	for (int i = 0; i < m_active_collider_group.size(); i++)
	{
		if (m_active_collider_group[i] == group_name)
			return;
	}
	m_active_collider_group.push_back(group_name);
	handle_collisions(true);
}

void CComponent::add_collisionprofile(CColliderBase * base)
{
	for (int i = 0; i < m_active_collider.size(); i++)
	{
		if (m_active_collider[i] == base)
			return;
	}
	m_active_collider.push_back(base);
	handle_collisions(true);
}

void CComponent::rem_collisionprofile(std::string group_name)
{
	for (int i = 0; i < m_active_collider_group.size(); i++)
	{
		if (m_active_collider_group[i] == group_name)
		{
			m_active_collider_group.erase(m_active_collider_group.begin() + i);
			return;
		}
	}
}

void CComponent::rem_collisionprofile(CColliderBase * base)
{
	for (int i = 0; i < m_active_collider.size(); i++)
	{
		if (m_active_collider[i] == base)
		{
			m_active_collider.erase(m_active_collider.begin() + i);
			return;
		}
	}
}
