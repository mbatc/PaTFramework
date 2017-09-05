#include "CColliderBase.h"

CColliderBase::CColliderBase()
{
}

CColliderBase::~CColliderBase()
{
}

unsigned int CColliderBase::update()
{
	return 0;
}

CCollisionData CColliderBase::get_collisiondata(int index)
{
	if (index < 0 || index >= m_data.size())
		return CCollisionData();

	return m_data[index];
}