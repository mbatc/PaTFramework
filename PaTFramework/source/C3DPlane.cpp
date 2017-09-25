#include "C3DPlane.h"

CVector3 C3DPlane::get_point(unsigned int calculated_point, CVector3 known_points)
{
	switch (calculated_point)
	{
	case PLANE_POINT_X:
		known_points.x = (m_scalar_values.w - m_scalar_values.y*known_points.y - m_scalar_values.z*known_points.z)/m_scalar_values.x;
	case PLANE_POINT_Y:
	case PLANE_POINT_Z:
	default:
		return CVector3();
	};

	return known_points;
}
