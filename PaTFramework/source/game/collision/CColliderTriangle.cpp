#include "CColliderTriangle.h"

int CColliderTriangle::_derived_id;

void CColliderTriangle::init(CVector3 p1, CVector3 p2, CVector3 p3)
{
	m_points[0] = p1;
	m_points[1] = p2;
	m_points[2] = p3;
	update_plane();
}

CVector3 CColliderTriangle::get_vertex_transformed(int index)
{
	if (index < 0 || index >= 3)
		return CVector3();

	return Point_transform(m_points[index], get_transform().get_translation(), 
		get_transform().get_rotation(),
		get_transform().get_scale());
}

C3DPlane CColliderTriangle::get_closestside(CVector3 to)
{
	update_plane();
	return m_plane;
}

void CColliderTriangle::update_plane()
{
	m_plane.set_point(get_vertex_transformed(0), PLANE_POINT_1);
	m_plane.set_point(get_vertex_transformed(1), PLANE_POINT_2);
	m_plane.set_point(get_vertex_transformed(2), PLANE_POINT_3);
	m_plane.update_plane();
}