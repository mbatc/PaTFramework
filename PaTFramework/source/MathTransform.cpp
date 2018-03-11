#include "MathTransform.h"
#include <float.h>

#include <vector>

V_RECT RotateRectangle(RECT r, CVector2 a, float rot)
{
	V_RECT ret;

	ret.TL = Point_rotate(CVector2(r.left, r.top), a, rot);
	ret.TR = Point_rotate(CVector2(r.right, r.top), a, rot);
	ret.BL = Point_rotate(CVector2(r.left, r.bottom), a, rot);
	ret.BR = Point_rotate(CVector2(r.right, r.bottom), a, rot);

	return ret;
}

CVector2 Point_rotate(CVector2 p, CVector2 a, float rot)
{
	CVector2 ret;
	float s = sinf(rot);
	float c = cosf(rot);

	ret.x = p.x - a.x;
	ret.y = p.y - a.y;

	float xnew = ret.x * c - ret.y * s;
	float ynew = ret.x * s + ret.y * c;

	ret.x = xnew + a.x;
	ret.y = ynew + a.y;

	return ret;
}

CVector3 Point_rotate(CVector3 p, CVector3 a, CVector3 rot)
{
	CVector3 temp = p - a;

	float x = cosf(rot.y)*temp.x + sinf(rot.y)*sinf(rot.x)*temp.y -
		sinf(rot.y)*cosf(rot.x)*temp.z;
	float y = cosf(rot.x)*temp.y + sinf(rot.x)*temp.z;
	float z = sinf(rot.y)*temp.x + cosf(rot.y)*-sin(rot.x)*temp.y +
		cosf(rot.y)*cosf(rot.x)*temp.z;

	return CVector3(x,y,z) + a;
}

CVector3 Point_transform(CVector3 p, CVector3 tran, CVector3 rot, CVector3 scl)
{
	CVector3 ret = p;
	ret *= scl;
	ret = Point_rotate(ret, CVector3(), rot);
	ret += tran;
	return ret;
}

float Vector_angleBetween(CVector3 a, CVector3 b)
{
	return acos(a.dot(b)/(a.Magnitude()*b.Magnitude()));
}

bool Line_getIntercept(CVector2 & i, CLine2D a, CLine2D b)
{
	return false;
}

bool Line_getIntercept(CVector3 & i, CLine3D a, CLine3D b)
{
	return false;
}

bool Line_getIntercept(CVector3 & i, CLine3D l, C3DPlane p)
{
	CVector3 n = p.get_normal();
	CVector3 line_coef = l.get_dir();

	//Check if the line and plane are parallel before continuing... if they are there will be no collision
	if (n.dot(line_coef) == 0)
		return false;

	CVector4 scalar_coef = p.get_scalar();
	CVector3 scalar_const = p.get_point(PLANE_POINT_1);

	//Scalar equation is in the form -
	//	scalar_coef.x(x - scalar_const.x) + scalar_coef.y(y - scalar_const.y) + scalar_coef.z(z - scalar_const.z) = scalar_coef.w
	
	CVector3 line_const = l.get_a();
	//Line equation is in the form
	//	v(t) = <line_coef.x*t + line_const.x, line_coef.y*t + line_const.y, line_coef.z*t + line_const.z>

	//Plug these into scalar equation for each component
	// scalar_coef.x(line_coef.x*t + line_const.x) +
	// scalar_coef.y(line_coef.y*t + line_const.y) + 
	// scalar_coef.z(line_coef.z*t + line_const.z) = scalar_coef.w 
	
	//Solve for t - Rearranging
	// scalar_coef.x*line_coef.x*t + line_const.x*scalar_coef.x +
	// scalar_coef.y*line_coef.y*t + line_const.y*scalar_coef.y +
	// scalar_coef.z*line_coef.z*t + line_const.z*scalar_coef.z = scalar_coef.w
	
	// (scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z)*t = 
	//		scalar_coef.w - line_const.x*scalar_coef.x
	//  				  - line_const.y*scalar_coef.y 
	//					  - line_const.z*scalar_coef.z)

	// (scalar_coef.w	- line_const.x*scalar_coef.x 
	//  				- line_const.y*scalar_coef.y 
	//					- line_const.z*scalar_coef.z) / 
	// (scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z) = t

	//FINAL EQUATION for 't'
	float t = 
		(scalar_coef.w - 

		line_const.x*scalar_coef.x -
		line_const.y*scalar_coef.y -
		line_const.z*scalar_coef.z ) 
		/
		(scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z);

	//Use 't' to find the point on the line
	i = CVector3(l.get_XatT(t), l.get_YatT(t), l.get_ZatT(t));
	return true;
}

float Plane_getDistanceFromPoint(C3DPlane plane, CVector3 point)
{
	point = Plane_projectPoint(plane, point) - point;
	return point.Magnitude();
}

float Plane_getDistanceFromPoint(C3DPlane plane, CVector3 point, CVector3 min, CVector3 max, float min_padding, float max_padding)
{
	min += min_padding;
	max += max_padding;

	CVector3 point_onplane = Plane_projectPoint(plane, point);
	if (point_onplane.x < min.x || point_onplane.y < min.y || point_onplane.z < min.z ||
		point_onplane.x > max.x || point_onplane.y > max.y || point_onplane.z > max.z)
		return FLT_MAX;

	point = point_onplane - point;
	return point.Magnitude();
}

CVector3 Plane_projectPoint(C3DPlane plane, CVector3 point)
{
	CLine3D line(point, point + plane.get_normal());
	CVector3 intercept;
	if (!Line_getIntercept(intercept, line, plane))
		return CVector3();

	return intercept;
}

CVector3 Transform_lerp(CVector3 to, CVector3 from, float factor)
{
	return from + CVector3(to-from)*factor;
}

CVector2 Transform_lerp(CVector2 to, CVector2 from, float factor)
{
	CVector3 v = Transform_lerp(CVector3(to.x, to.y, 0.0f), CVector3(from.x, from.y, 0.0f), factor);
	return CVector2(v.x, v.y);
}

bool Collision_AABBCheck(CVector3 min, CVector3 max, CVector3 point)
{
	return Collision_AABBCheck(min,max,point,point);
}

bool Collision_AABBCheck(CVector3 min_1, CVector3 max_1, CVector3 min_2, CVector3 max_2)
{
	return (	min_1.x <= max_2.x && 
				min_2.x <= max_1.x && 
				min_1.y <= max_2.y &&
				min_2.y <= max_1.y && 
				min_1.z <= max_2.z &&
				min_2.z <= max_1.z );
}

bool Collision_SphereAABBCheck(CVector3 center, float radius, CVector3 _min, CVector3 _max)
{
	return Collision_SphereCheck(center, radius, CVector3(
		max(_min.x, min(center.x, _max.x)),
		max(_min.y, min(center.y, _max.y)),
		max(_min.z, min(center.z, _max.z))
	));
}

bool Collision_SphereCheck(CVector3 center, float radius, CVector3 point)
{
	CVector3 d = point - center;
	return d.Magnitude() < radius;
}

bool Collision_TriAABBCheck(C3DPlane tri, C3DPlane* col_planes)
{
	CLine3D tri_line[3] = {
		CLine3D(tri.get_point(0), tri.get_point(1)),
		CLine3D(tri.get_point(1), tri.get_point(2)),
		CLine3D(tri.get_point(2), tri.get_point(0))
	};

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			CVector3 intercept;
			if (!Line_getIntercept(intercept, tri_line[j], col_planes[i]))
				continue;
			if (!tri_line[j].within_givenpoints(intercept))
				continue;
			CVector3 min; col_planes[i].get_min(min);
			CVector3 max; col_planes[i].get_max(max);

			if (!Collision_AABBCheck(min, max, intercept))
				continue;

			return true;
		}
	}

	return false;
}

bool Collision_TriAABBCheck(C3DPlane tri, CVector3 min, CVector3 max)
{
	// First Check if any of the planes points are within the boundary 
	// *least expensive collision check - can exit early if true

	for (int i = 0; i < 3; i++)
		if (Collision_AABBCheck(min, max, tri.get_point(i)))
			return true;

	// Create an array of 3DLines from the min/max vectors and check if they intersect the 
	// triangles plane within the boundary points

	CVector3 points[8] = {
		CVector3(min.x, min.y, min.z),
		CVector3(min.x, min.y, max.z),
		CVector3(min.x, max.y, min.z),
		CVector3(min.x, max.y, max.z),
		CVector3(max.x, min.y, min.z),
		CVector3(max.x, max.y, max.z),
		CVector3(max.x, min.y, min.z),
		CVector3(max.x, min.y, max.z) };

	std::vector<CLine3D> lines;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			if (j == i)
				continue;

			int nChanges = 0;
			for (int a = 0; a < 3; a++)
			{
				if (points[i].get(a) != points[j].get(a))
					nChanges++;
			}

			if (nChanges == 1)
			{
				CLine3D l = CLine3D(points[i], points[j]);

				bool unique = true;
				for (int ii = 0; ii < lines.size(); ii++)
				{
					if (lines[ii].get_a() == l.get_b() &&
						lines[ii].get_b() == l.get_a())
						unique = false;
				}

				if(unique)
					lines.push_back(l);
			}
		}
	}

	//Now check for intesections with the triangle
	for (int i = 0; i < lines.size(); i++)
	{
		CVector3 intercept;
		if (!Line_getIntercept(intercept, lines[i], tri))
			continue;

		if(!lines[i].within_givenpoints(intercept))
			continue;

		for (int j = 0; j < 3; j++)
		{
			int a = j % 3; int b = (j + 1) % 3; int c = (j + 2) % 3;
			float angle = abs(Vector_angleBetween(tri.get_point(a) - tri.get_point(b), tri.get_point(c) - tri.get_point(b)));

			float check_1 = abs(Vector_angleBetween(tri.get_point(a) - tri.get_point(b), intercept - tri.get_point(b)));
			float check_2 = abs(Vector_angleBetween(tri.get_point(c) - tri.get_point(b), intercept - tri.get_point(b)));

			if (check_1 > angle || check_2 > angle)
				break;

			if (j == 2)
				return true;
		}
	}

	return false;
}

bool Collision_TriSphereCheck(C3DPlane tri, CVector3 center, float radius)
{
	return false;
}

bool Collision_TriCheck(C3DPlane tri, C3DPlane tri2)
{
	//Now check for intesections with the triangle
	for (int i = 0; i < 3; i++)
	{
		CVector3 intercept;
		Line_getIntercept(intercept, CLine3D(tri2.get_point(i), tri2.get_point((i + 1) % 3)), tri);

		for (int j = 0; j < 3; j++)
		{
			int a = j % 3; int b = (j + 1) % 3; int c = (j + 2) % 3;
			//Check within first triangle
			float angle = abs(Vector_angleBetween(tri.get_point(a) - tri.get_point(b), tri.get_point(c) - tri.get_point(b)));

			float check_1 = abs(Vector_angleBetween(tri.get_point(a) - tri.get_point(b), intercept - tri.get_point(b)));
			float check_2 = abs(Vector_angleBetween(tri.get_point(c) - tri.get_point(b), intercept - tri.get_point(b)));

			if (check_1 > angle || check_2 > angle)
				break;
			
			//Check within second triangle
			angle = abs(Vector_angleBetween(tri2.get_point(a) - tri2.get_point(b), tri2.get_point(c) - tri2.get_point(b)));

			check_1 = abs(Vector_angleBetween(tri2.get_point(a) - tri2.get_point(b), intercept - tri2.get_point(b)));
			check_2 = abs(Vector_angleBetween(tri2.get_point(c) - tri2.get_point(b), intercept - tri2.get_point(b)));

			if (check_1 > angle || check_2 > angle)
				break;

			if (j == 2)
				return true;
		}
	}

	return false;
}
