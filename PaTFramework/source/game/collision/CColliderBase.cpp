#include "CColliderBase.h"

CColliderBase::CColliderBase()
{
}

CColliderBase::~CColliderBase()
{
}

unsigned int CColliderBase::update()
{
	m_data.clear();
	return 0;
}

CCollisionData CColliderBase::get_collisiondata(int index)
{
	if (index < 0 || index >= m_data.size())
		return CCollisionData();

	return m_data[index];
}

CTransform CColliderBase::get_transform()
{
	return m_transform;
}

C3DPlane CColliderBase::get_closestside(CVector3 to)
{
	return C3DPlane(CVector3(),CVector3());
}

unsigned int CColliderBase::get_collisioncount()
{
	return m_data.size();
}

unsigned int CColliderBase::add_collision(CColliderBase * b)
{
	CCollisionData d;
	d.m_velocity = this->m_transform.get_translation() - 
		m_prev_transform.get_translation();
	d.pWith = b;
	d.m_collision_point = get_collision_point(b);

	m_data.push_back(d);
	return 0;
}

CVector3 CColliderBase::get_collision_point(CColliderBase * b)
{
	if (!b)
		return CVector3();

	CVector3 i;
	Line_getIntercept(i,
		CLine3D(b->get_transform().get_translation(),
			get_transform().get_translation()),

		b->get_closestside((b->get_transform().get_translation() - 
			get_transform().get_translation()) / 2));

	return i;
}
