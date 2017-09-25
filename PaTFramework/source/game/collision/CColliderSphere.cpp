#include "CColliderAABB.h"
#include "CColliderSphere.h"
#include "CCollisionEngine.h"

int CColliderSphere::_derived_id;

CColliderSphere::CColliderSphere()
{

}

CColliderSphere::~CColliderSphere()
{

}

C3DPlane CColliderSphere::get_closestside(CVector3 point)
{
	CVector3 normal = point - get_transform().get_translation();
	CVector3 unit_vec = normal / normal.Magnitude();
	CVector3 point_1 = unit_vec*m_radius;

	return C3DPlane(point_1, unit_vec);
}