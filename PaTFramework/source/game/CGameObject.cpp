#include "CGameObject.h"
#include "../MathTransform.h"
#include "../CD3DGraphics.h"

CGameObject::CGameObject(std::string name)
	:
	m_name(name)
{
	m_lerp_factor = 1.0f;
}

CGameObject::~CGameObject()
{}

void CGameObject::set_parent(CGameObject * parent)
{
	m_parent = parent;
	parent->add_child(this);
}

void CGameObject::add_child(CGameObject * child)
{
	if (!child)
		return;

	if (child->get_parent() != this)
		child->set_parent(this);
	
	for (int i = 0; i < m_child_list.size(); i++)
	{
		if (m_child_list[i] == child)
			return;
	}

	m_child_list.push_back(child);
}

void CGameObject::rem_child(CGameObject * child)
{
	for (int i = 0; i < m_child_list.size(); i++)
	{
		if (m_child_list[i] == child)
		{
			m_child_list[i]->set_parent(nullptr);
			m_child_list.erase(m_child_list.begin() + i);
			return;
		}
	}
}

unsigned int CGameObject::update()
{
	m_prev_local_transform = m_local_transform;
	m_prev_global_transform = m_global_transform;

	m_global_transform = m_local_transform;
	if (m_parent)
	{
		CTransform parent_trans = m_parent->get_global_transform();

		CVector3 rot = parent_trans.get_rotation();
		CVector3 pos = m_local_transform.get_translation();

		D3DXVECTOR3 dx_npos(pos.x, pos.y, pos.z);
		D3DXMATRIX transform_rot;
		D3DXMatrixRotationYawPitchRoll(&transform_rot, rot.y,
			rot.x,
			rot.z);

		//Rotate the object by the same amount as its parent
		D3DXVec3TransformNormal(&dx_npos, &dx_npos, &transform_rot);
		CVector3 npos(dx_npos.x, dx_npos.y, dx_npos.z);
		npos += parent_trans.get_translation();

		m_global_transform.get_scale() *= parent_trans.get_scale();

		m_global_transform.get_translation() = Transform_lerp(npos,
			m_prev_global_transform.get_translation(),m_lerp_factor);

		m_global_transform.get_rotation() = Transform_lerp(m_global_transform.get_rotation() + parent_trans.get_rotation(),
			m_prev_global_transform.get_rotation(),m_lerp_factor);
	}

	for (int i = 0; i < m_component.size(); i++)
	{
		if (m_component[i])
			_last_error = m_component[i]->update();
	}
	return _last_error;
}

unsigned int CGameObject::render(CD3DGraphics& gfx)
{
	for (int i = 0; i < m_component.size(); i++)
	{
		if (m_component[i])
			_last_error = m_component[i]->render(gfx);
	}
	return _last_error;
}

unsigned int CGameObject::startup()
{
	for (int i = 0; i < m_component.size(); i++)
	{
		if (m_component[i])
			_last_error = m_component[i]->startup();
	}
	return _last_error;
}

unsigned int CGameObject::add_component(CComponent * comp)
{
	if (!comp)
		return 1;

	comp->set_parent(this);
	m_component.push_back(comp);
	return 0;
}

void CGameObject::rem_component(int index)
{
	if (index < 0 || index >= m_component.size())
		return;

	m_component.erase(m_component.begin() + index);
}

void CGameObject::rem_component(std::string name)
{
	for (int i = 0; i < m_component.size(); i++)
	{
		if (m_component[i]->get_name() == name)
		{
			rem_component(i);
			return;
		}
	}
}

void CGameObject::lerp_to_parent(float factor)
{
	m_lerp_factor = factor;
}
