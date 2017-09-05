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
private:
	CVector3 min;
	CVector3 max;

	C3DPlane sides[8];
};