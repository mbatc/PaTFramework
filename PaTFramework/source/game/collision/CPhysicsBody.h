#pragma once

#include "../CTransform.h"

class CPhysicsBody
{
public:
	CPhysicsBody();
	~CPhysicsBody();


private:
	CVector3 m_velocity_angular;
	CVector3 m_velocity;

	CTransform m_transform;
};