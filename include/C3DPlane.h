#pragma once

#include "CVector3.h"
#include "CVector4.h"
#include "CLine.h"

#define PLANE_POINT_1 0
#define PLANE_POINT_2 1
#define PLANE_POINT_3 2
#define PLANE_POINT_4 3

#define PLANE_MAX_POINTS 4

class C3DPlane
{
public:
	C3DPlane(CVector3 p_1, CVector3 p_2, CVector3 p_3, CVector3 p_4 = CVector3()) {
		set_point(p_1, PLANE_POINT_1);
		set_point(p_2, PLANE_POINT_2);
		set_point(p_3, PLANE_POINT_3);
		set_point(p_4, PLANE_POINT_4);
		update_plane();
	}

	C3DPlane(CVector3 p_1, CVector3 normal) {
		set_point(p_1, PLANE_POINT_1);
		set_normal(normal);
		update_plane(true);
	}

	C3DPlane() {
		set_point(CVector3(), PLANE_POINT_1);
		set_point(CVector3(), PLANE_POINT_2);
		set_point(CVector3(), PLANE_POINT_3);
		set_point(CVector3(), PLANE_POINT_4);
		update_plane();
	}
	
	void		set_point(CVector3 p, int index) { if (!valid_index(index)) return;  m_point[index] = p; }
	void		set_normal(CVector3 norm) { m_normal = norm; }
	CVector3	get_normal() { return m_normal; }
	CVector3	get_point(int index) { if (!valid_index(index))return CVector3(); return m_point[index]; }
	
	//returns vector containing coefficients of the planes scalar equation
	CVector4	get_scalar() { return m_scalar_values; }

	void update_plane(bool from_normal=false) {
		if (!from_normal)
		{
			CVector3 v1 = m_point[0] - m_point[1];
			CVector3 v2 = m_point[2] - m_point[1];

			//calculate normal using cross product
			m_normal.x = (v1.y * v2.z) - (v1.z*v2.y);
			m_normal.y = -(v1.x * v2.z) + (v1.z*v2.x);
			m_normal.z = (v1.x * v2.y) - (v1.y*v2.x);
		}

		//calculate scalar values using normal in the form
		// ax + by + zx = w
		m_scalar_values.x = m_normal.x;
		m_scalar_values.y = m_normal.y;
		m_scalar_values.z = m_normal.z;
		m_scalar_values.w = -(m_normal.x * m_point[0].x) + (m_normal.y*m_point[0].y) - (m_normal.z*m_point[0].z);
	}
private:
	bool valid_index(int index) {
		return !(index < 0 || index >= PLANE_MAX_POINTS);
	}
	
	CVector3 m_normal;
	CVector4 m_scalar_values;
	CVector3 m_point[PLANE_MAX_POINTS];
};