#include "CColliderTriangle.h"

CVector3 CColliderTriangle::get_vertex_transformed(int index)
{
	if (index < 0 || index >= 3)
		return CVector3();

	return Point_transform(m_points[index], m_transform.get_translation(), 
		m_transform.get_rotation(),
		m_transform.get_scale());
}
