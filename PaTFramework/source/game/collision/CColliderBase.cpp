#include "CColliderBase.h"
#include "../../CLog.h"
#include "../CGameObject.h"

int CColliderBase::_base_id;

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

unsigned int CColliderBase::collision()
{
	if (m_data.size() == 0 || !m_handleObj)
		return 0;

	return m_handleObj->collision(this,m_data);
}

CCollisionData CColliderBase::get_collisiondata(int index)
{
	if (index < 0 || index >= m_data.size())
		return CCollisionData();

	return m_data[index];
}

CCollisionData CColliderBase::calculate_collision(CColliderBase * collider)
{
	CCollisionData d;
	d.m_velocity = this->m_transform.get_translation() -
		m_prev_transform.get_translation();
	d.m_collision_point = get_collision_point(collider);
	d.m_plane = get_closestside(d.m_collision_point);

	d.pWith = collider;
	d.with_collision_point = collider->get_collision_point(this);
	d.with_plane = collider->get_closestside(d.with_collision_point);
	d.with_velocity = collider->get_transform().get_translation() - collider->m_prev_transform.get_translation();

	return d;
}

CTransform& CColliderBase::get_transform()
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

unsigned int CColliderBase::add_collision(CCollisionData data)
{	
	m_data.push_back(data);
	return 0;
}

unsigned int CColliderBase::set_gameobject(CGameObject * obj)
{
	Log(this,DEBUGLOG_LEVEL_INFO,"Game Object handling collisions updated from (ptr=%p) to (ptr=%p)", m_handleObj, obj);
	m_handleObj = obj;
	m_transform = m_handleObj->get_transform();
	m_prev_transform = m_handleObj->get_transform();
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
