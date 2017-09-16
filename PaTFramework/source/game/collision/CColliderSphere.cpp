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

unsigned int CColliderSphere::check_collision(CColliderBase* col)
{
	if (CCollisionEngine::is_type<CColliderAABB>(col))
	{
		CColliderAABB* c = (CColliderAABB*)col;
		if (Collision_SphereAABBCheck(get_transform().get_translation(),
			m_radius,c->get_boundmin(), c->get_boundmax()))
		{
			add_collision(col);
			col->add_collision(this);
		}
	}

	if (CCollisionEngine::is_type<CColliderSphere>(col))
	{
		CColliderSphere* c = (CColliderSphere*)col;
		if (Collision_SphereCheck(get_transform().get_translation(), m_radius + c->get_radius(),
			c->get_transform().get_translation()))
		{
			add_collision(col);
			col->add_collision(this);
		}
	}
	return 0;
}

C3DPlane CColliderSphere::get_closestside(CVector3 point)
{
	CVector3 normal = point - get_transform().get_translation();
	CVector3 unit_vec = normal / normal.Magnitude();
	CVector3 point_1 = unit_vec*m_radius;

	return C3DPlane(point_1, unit_vec);
}