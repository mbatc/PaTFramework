#include "CColliderAABB.h"

CColliderAABB::CColliderAABB(): CColliderBase_inherit() {

}

CColliderAABB::~CColliderAABB() {

}

void CColliderAABB::set_bounds(CVector3 min, float width, float height, float depth)
{
	set_bounds(min, CVector3(width, height, depth));
}

void CColliderAABB::set_bounds(CVector3 min, CVector3 max)
{
}

unsigned int CColliderAABB::check_collision(CColliderBase * collider)
{
	return 0;
}

C3DPlane CColliderAABB::get_closestside(CVector3 to)
{
	return C3DPlane();
}
