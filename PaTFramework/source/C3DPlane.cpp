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

int	C3DPlane::get_min(CVector3& point)
{
	float x = FLT_MAX, y = FLT_MAX, z = FLT_MAX;
	for (int i = 1; i < PLANE_MAX_POINTS; i++)
	{
		if (points_set[i])
		{
			x = min(x, m_point[i].x);
			y = min(y, m_point[i].y);
			z = min(z, m_point[i].z);
		}
	}
	point = CVector3(x, y, z);
	return nPointsSet();
}

int	C3DPlane::get_max(CVector3& point)
{
	float x = -FLT_MAX, y = -FLT_MAX, z = -FLT_MAX;
	for (int i = 1; i < PLANE_MAX_POINTS; i++)
	{
		if (points_set[i])
		{
			x = max(x, m_point[i].x);
			y = max(y, m_point[i].y);
			z = max(z, m_point[i].z);
		}
	}
	point = CVector3(x, y, z);
	return nPointsSet();
}

int C3DPlane::nPointsSet()
{
	int count = 0;
	for (int i = 0; i < PLANE_MAX_POINTS; i++)
	{
		if (points_set[i])
			count++;
	}
	return count;
}
