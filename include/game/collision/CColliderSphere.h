#pragma once

#include "CColliderBase.h"

derive_ColliderType(CColliderSphere)
{
public:
	CColliderSphere();
	~CColliderSphere();

	void set_radius(float r) { m_radius = r; }
	float get_radius() { return m_radius; }
	C3DPlane		get_closestside(CVector3 to) override;
private:
	float m_radius;
};