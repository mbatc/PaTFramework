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

C3DPlane CColliderAABB::get_closestside(CVector3 to)
{
	CLine3D l(to,get_transform().get_translation());
	int index = -1;
	float dist = FLT_MAX;
	for(int i = 0; i < sizeof(sides) / sizeof(C3DPlane); i++)
	{
		CVector3 col_point;
		if (!Line_getIntercept(col_point, l, sides[i]))
			continue;

		float temp = (col_point - to).Magnitude();
		if (temp < dist && Collision_AABBCheck(m_min + get_transform().get_translation(), 
			m_max + get_transform().get_translation(), col_point))
		{
			index = i;
			dist = temp;
		}
	}

	if (index < 0)
		return C3DPlane();

	return sides[index];
}

CVector3 CColliderAABB::get_collision_point(CColliderBase* base)
{
	if (CCollisionEngine::is_type<CColliderAABB>(base))
	{
		float dist = 0;
		CVector3 point_a;
		CVector3 point_b;
		CColliderAABB* c = (CColliderAABB*)base;
		bool point_found = false;

		std::vector<CVector3> points_inbounds_b;
		for (int i = 0; i < AABB_VERT_COUNT; i++)
		{
			CVector3 temp_b = get_vertex_translated(i);
			if (!Collision_AABBCheck(c->get_boundmin(), c->get_boundmax(), temp_b))
				continue;
			points_inbounds_b.push_back(temp_b);

			for (int j = 0; j < AABB_VERT_COUNT; j++)
			{
				CVector3 temp_a = c->get_vertex_translated(j);

				if (!Collision_AABBCheck(get_boundmin(),get_boundmax(), temp_a))
					continue;

				float temp_dist = (temp_a - temp_b).Magnitude();
				if (temp_dist > dist)
				{
					dist = temp_dist;
					point_a = temp_a;
					point_b = temp_b;
					point_found = true;
				}
			}
		}

		if(point_found)
			return (point_a + point_b) / 2;


		if (point_found && points_inbounds_b.size() > 0)
		{
			CVector3 avg_point;
			for (int i = 0; i < points_inbounds_b.size(); i++)
			{
				avg_point += points_inbounds_b[i];
			}
			avg_point = avg_point/points_inbounds_b.size();

			CVector3 change = avg_point - points_inbounds_b[0];
			change = change/change.Magnitude();
			change = CVector3(1.0f - change.x, 1.0f - change.y, 1.0f - change.z);

			CVector3 avg_with_point;
			std::vector<CVector3> closest_points;
			float dist = FLT_MAX;
			for (int i = 0; i < 4 - points_inbounds_b.size(); i++)
			{
				closest_points.push_back(CVector3());
				float dist = FLT_MAX;
				for (int j = 0; j < AABB_VERT_COUNT; j++)
				{
					CVector3 temp_p = c->get_vertex_translated(j);
					float temp = (avg_point - temp_p).Magnitude();
					if (temp < dist)
					{
						bool already_exists = false;
						if (closest_points.size() > 0)
						{
							for (int k = 0; k < closest_points.size(); k++)
							{
								if (closest_points[k] == temp_p)
									already_exists = true;
							}
						}

						if (!already_exists)
						{
							dist = temp;
							closest_points[i] = temp_p;
						}
					}
				}
			}

			for (int i = 0; i < closest_points.size(); i++)
			{
				avg_with_point += closest_points[i];
			}

			avg_with_point = avg_with_point/closest_points.size();

			CVector3 with_change = avg_with_point - closest_points[0];
			with_change = with_change/with_change.Magnitude();
			with_change = CVector3(1.0f - with_change.x, 1.0f - with_change.y, 1.0f - with_change.z);

			avg_point = (avg_point*change + avg_with_point*with_change) / 2;
			return avg_point;
		}
	}

	CVector3 point = base->get_transform().get_translation();
	CLine3D l(base->get_transform().get_translation(), get_transform().get_translation());
	CVector3 collision_point;
	float dist = FLT_MAX;
	for (int i = 0; i < sizeof(sides) / sizeof(C3DPlane); i++)
	{
		CVector3 temp_col_point;
		if (!Line_getIntercept(temp_col_point, l, sides[i]))
			continue;

		float temp = (temp_col_point - point).Magnitude();
		if (temp < dist && Collision_AABBCheck(m_min + get_transform().get_translation(), 
			m_max+get_transform().get_translation(), temp_col_point))
		{
			collision_point = temp_col_point;
			dist = temp;
		}
	}

	return collision_point;
}

void CColliderAABB::update_sides()
{
	//Update points
	points[0] = CVector3(m_max.x, m_max.y, m_max.z);
	points[1] = CVector3(m_max.x, m_max.y, m_min.z);
	points[2] = CVector3(m_max.x, m_min.y, m_min.z);
	points[3] = CVector3(m_max.x, m_min.y, m_max.z);

	points[4] = CVector3(m_min.x, m_max.y, m_max.z);
	points[5] = CVector3(m_min.x, m_max.y, m_min.z);
	points[6] = CVector3(m_min.x, m_min.y, m_min.z);
	points[7] = CVector3(m_min.x, m_min.y, m_max.z);
	//--------------------------------------------------------

	//TOP PLANE n(0,1,0)
	sides[0].set_point(CVector3(m_max.x, m_max.y, m_max.z), 0);
	sides[0].set_point(CVector3(m_max.x, m_max.y, m_min.z), 1);
	sides[0].set_point(CVector3(m_min.x, m_max.y, m_min.z), 2);
	sides[0].set_point(CVector3(m_min.x, m_max.y, m_max.z), 3);
	sides[0].update_plane();

	//BOTTOM PLANE n(0,-1,0)
	sides[1].set_point(CVector3(m_max.x, m_min.y, m_max.z), 3);
	sides[1].set_point(CVector3(m_max.x, m_min.y, m_min.z), 2);
	sides[1].set_point(CVector3(m_min.x, m_min.y, m_min.z), 1);
	sides[1].set_point(CVector3(m_min.x, m_min.y, m_max.z), 0);
	sides[1].update_plane();

	//RIGHT PLANE n(1,0,0)
	sides[2].set_point(CVector3(m_max.x, m_min.y, m_max.z), 3);
	sides[2].set_point(CVector3(m_max.x, m_min.y, m_min.z), 2);
	sides[2].set_point(CVector3(m_max.x, m_max.y, m_min.z), 1);
	sides[2].set_point(CVector3(m_max.x, m_max.y, m_max.z), 0);
	sides[2].update_plane();

	//LEFT PLANE n(-1,0,0)
	sides[3].set_point(CVector3(m_min.x, m_min.y, m_max.z), 0);
	sides[3].set_point(CVector3(m_min.x, m_min.y, m_min.z), 1);
	sides[3].set_point(CVector3(m_min.x, m_max.y, m_min.z), 2);
	sides[3].set_point(CVector3(m_min.x, m_max.y, m_max.z), 3);
	sides[3].update_plane();
	
	//NEAR PLANE n(0,0-1)
	sides[4].set_point(CVector3(m_min.x, m_min.y, m_min.z), 0);
	sides[4].set_point(CVector3(m_max.x, m_min.y, m_min.z), 1);
	sides[4].set_point(CVector3(m_max.x, m_max.y, m_min.z), 2);
	sides[4].set_point(CVector3(m_min.x, m_max.y, m_min.z), 3);
	sides[4].update_plane();

	//FAR PLANE n(0,0,1)
	sides[5].set_point(CVector3(m_min.x, m_min.y, m_max.z), 3);
	sides[5].set_point(CVector3(m_max.x, m_min.y, m_max.z), 2);
	sides[5].set_point(CVector3(m_max.x, m_max.y, m_max.z), 1);
	sides[5].set_point(CVector3(m_min.x, m_max.y, m_max.z), 0);
	sides[5].update_plane();
}

void CColliderAABB::update_sides_trans()
{
	CVector3 max = get_boundmax();
	CVector3 min = get_boundmin();

	//TOP PLANE n(0,1,0)
	sides_translated[0].set_point(CVector3(max.x, max.y, max.z), 0);
	sides_translated[0].set_point(CVector3(max.x, max.y, min.z), 1);
	sides_translated[0].set_point(CVector3(min.x, max.y, min.z), 2);
	sides_translated[0].set_point(CVector3(min.x, max.y, max.z), 3);
	sides_translated[0].update_plane();

	//BOTTOM PLANE n(0,-1,0)
	sides_translated[1].set_point(CVector3(max.x, min.y, max.z), 3);
	sides_translated[1].set_point(CVector3(max.x, min.y, min.z), 2);
	sides_translated[1].set_point(CVector3(min.x, min.y, min.z), 1);
	sides_translated[1].set_point(CVector3(min.x, min.y, max.z), 0);
	sides_translated[1].update_plane();

	//RIGHT PLANE n(1,0,0)
	sides_translated[2].set_point(CVector3(max.x, min.y, max.z), 3);
	sides_translated[2].set_point(CVector3(max.x, min.y, min.z), 2);
	sides_translated[2].set_point(CVector3(max.x, max.y, min.z), 1);
	sides_translated[2].set_point(CVector3(max.x, max.y, max.z), 0);
	sides_translated[2].update_plane();

	//LEFT PLANE n(-1,0,0)
	sides_translated[3].set_point(CVector3(min.x, min.y, max.z), 0);
	sides_translated[3].set_point(CVector3(min.x, min.y, min.z), 1);
	sides_translated[3].set_point(CVector3(min.x, max.y, min.z), 2);
	sides_translated[3].set_point(CVector3(min.x, max.y, max.z), 3);
	sides_translated[3].update_plane();

	//NEAR PLANE n(0,0-1)
	sides_translated[4].set_point(CVector3(min.x, min.y, min.z), 0);
	sides_translated[4].set_point(CVector3(max.x, min.y, min.z), 1);
	sides_translated[4].set_point(CVector3(max.x, max.y, min.z), 2);
	sides_translated[4].set_point(CVector3(min.x, max.y, min.z), 3);
	sides_translated[4].update_plane();

	//FAR PLANE n(0,0,1)
	sides_translated[5].set_point(CVector3(min.x, min.y, max.z), 3);
	sides_translated[5].set_point(CVector3(max.x, min.y, max.z), 2);
	sides_translated[5].set_point(CVector3(max.x, max.y, max.z), 1);
	sides_translated[5].set_point(CVector3(min.x, max.y, max.z), 0);
	sides_translated[5].update_plane();
}
