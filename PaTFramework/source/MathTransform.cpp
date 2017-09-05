#include "MathTransform.h"

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
	return CVector3();
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
	// scalar_coef.x(line_coef.x*t + line_const.x - scalar_const.x) +
	// scalar_coef.y(line_coef.y*t + line_const.y - scalar_const.y) + 
	// scalar_coef.z(line_coef.z*t + line_const.z - scalar_const.z) = scalar_coef.w 
	
	//Solve for t - Rearranging
	// scalar_coef.x*line_coef.x*t + line_const.x*scalar_coef.x - scalar_const.x*scalar_coef.x +
	// scalar_coef.y*line_coef.y*t + line_const.y*scalar_coef.y - scalar_const.x*scalar_coef.y +
	// scalar_coef.z*line_coef.z*t + line_const.z*scalar_coef.z - scalar_const.x*scalar_coef.z = scalar_coef.w
	
	// (scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z)*t = 
	//		scalar_coef.w - line_const.x*scalar_coef.x + scalar_const.x*scalar_coef.x 
	//  				  - line_const.y*scalar_coef.y + scalar_const.x*scalar_coef.y 
	//					  - line_const.z*scalar_coef.z + scalar_const.x*scalar_coef.z)

	// (scalar_coef.w - line_const.x*scalar_coef.x + scalar_const.x*scalar_coef.x 
	//  				  - line_const.y*scalar_coef.y + scalar_const.x*scalar_coef.y 
	//					  - line_const.z*scalar_coef.z + scalar_const.x*scalar_coef.z) / 
	// (scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z) = t

	//FINAL EQUATION for 't'
	float t = 
		(scalar_coef.w - 

		line_const.x*scalar_coef.x + scalar_const.x*scalar_coef.x -
		line_const.y*scalar_coef.y + scalar_const.x*scalar_coef.y -
		line_const.z*scalar_coef.z + scalar_const.x*scalar_coef.z ) 
		/
		(scalar_coef.x*line_coef.x + scalar_coef.y*line_coef.y + scalar_coef.z*line_coef.z);

	//Use 't' to find the point on the line
	i = CVector3(l.get_XatT(t), l.get_YatT(t), l.get_ZatT(t));
	return true;
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
	return (	min_1.x < max_2.x && 
				min_2.x < max_1.x && 
				min_1.y < max_2.y &&
				min_2.y < max_1.y && 
				min_1.z < max_2.z &&
				min_2.z < max_2.z );
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
