#include "CColliderAABB.h"
#include "CCollisionEngine.h"

int CColliderAABB::_derived_id;

CColliderAABB::CColliderAABB() : CColliderBase_inherit(){

}

CColliderAABB::~CColliderAABB() {

}

void CColliderAABB::set_bounds(CVector3 min, float width, float height, float depth)
{
	set_bounds(min, CVector3(width, height, depth));
}

void CColliderAABB::set_bounds(CVector3 min, CVector3 max)
{
	m_min = min;
	m_max = max;

	update_sides();
}

unsigned int CColliderAABB::check_collision(CColliderBase * collider)
{
	if (CCollisionEngine::is_type<CColliderAABB>(collider))
	{
		CColliderAABB* c = (CColliderAABB*)collider;
		if (Collision_AABBCheck(c->m_min, c->m_max, m_min, m_max))
		{
			add_collision(collider);
			collider->add_collision(this);
		}
	}
	return 0;
}

C3DPlane CColliderAABB::get_closestside(CVector3 to)
{
	float dist = Plane_getDistanceFromPoint(sides[0], to);
	int index = 0;
	for (int i = 1; i < sizeof(sides) / sizeof(C3DPlane); i++)
	{
		float temp = Plane_getDistanceFromPoint(sides[i], to);
		if (temp < dist)
		{
			dist = temp;
			index = i;
		}
	}

	return sides[index];
}

void CColliderAABB::update_sides()
{
	sides[0].set_point(CVector3(m_max.x, m_max.y, m_max.z), 0);
	sides[0].set_point(CVector3(m_max.x, m_max.y, m_min.z), 1);
	sides[0].set_point(CVector3(m_min.x, m_max.y, m_min.z), 2);
	sides[0].set_point(CVector3(m_min.x, m_max.y, m_max.z), 3);
	sides[0].update_plane();

	sides[1].set_point(CVector3(m_max.x, m_min.y, m_max.z), 0);
	sides[1].set_point(CVector3(m_max.x, m_min.y, m_min.z), 1);
	sides[1].set_point(CVector3(m_min.x, m_min.y, m_min.z), 2);
	sides[1].set_point(CVector3(m_min.x, m_min.y, m_max.z), 3);
	sides[1].update_plane();

	sides[2].set_point(CVector3(m_max.x, m_min.y, m_max.z), 0);
	sides[2].set_point(CVector3(m_max.x, m_min.y, m_min.z), 1);
	sides[2].set_point(CVector3(m_max.x, m_max.y, m_min.z), 2);
	sides[2].set_point(CVector3(m_max.x, m_max.y, m_max.z), 3);
	sides[2].update_plane();

	sides[3].set_point(CVector3(m_min.x, m_min.y, m_max.z), 0);
	sides[3].set_point(CVector3(m_min.x, m_min.y, m_min.z), 1);
	sides[3].set_point(CVector3(m_min.x, m_max.y, m_min.z), 2);
	sides[3].set_point(CVector3(m_min.x, m_max.y, m_max.z), 3);
	sides[3].update_plane();
	
	sides[4].set_point(CVector3(m_min.x, m_min.y, m_min.z), 0);
	sides[4].set_point(CVector3(m_max.x, m_min.y, m_min.z), 1);
	sides[4].set_point(CVector3(m_max.x, m_max.y, m_min.z), 2);
	sides[4].set_point(CVector3(m_min.x, m_max.y, m_min.z), 3);
	sides[4].update_plane();

	sides[5].set_point(CVector3(m_min.x, m_min.y, m_max.z), 0);
	sides[5].set_point(CVector3(m_max.x, m_min.y, m_max.z), 1);
	sides[5].set_point(CVector3(m_max.x, m_max.y, m_max.z), 2);
	sides[5].set_point(CVector3(m_min.x, m_max.y, m_max.z), 3);
	sides[5].update_plane();
}