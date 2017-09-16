#pragma once

#include "CColliderBase.h"

derive_ColliderType(CColliderAABB)
{
public:
	CColliderAABB();
	~CColliderAABB();

	void set_bounds(CVector3 min, float width, float height, float depth);
	void set_bounds(CVector3 min, CVector3 max);

	unsigned int	check_collision(CColliderBase* collider) override;
	C3DPlane		get_closestside(CVector3 to) override;

	CVector3 get_boundmin() { return m_min+get_transform().get_translation(); }
	CVector3 get_boundmax() { return m_max+get_transform().get_translation(); }
private:
	void update_sides();

	CVector3 m_min;
	CVector3 m_max;

	C3DPlane sides[8];
};