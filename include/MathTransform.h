#pragma once

#include "CLine.h"
#include "C3DPlane.h"
#include "Vectors.h"
#include <Windows.h>

#define _USE_MATH_DEFINES
#include <math.h>

struct V_RECT
{
	CVector2 TL;
	CVector2 BL;
	CVector2 TR;
	CVector2 BR;
};

V_RECT RotateRectangle(RECT r, CVector2 a, float rot);

CVector2 Point_rotate(CVector2 p, CVector2 a, float rot);
CVector3 Point_rotate(CVector3 p, CVector3 a, CVector3 rot);

CVector3 Point_transform(CVector3 p, CVector3 tran, CVector3 rot = CVector3(), CVector3 scl = CVector3(1.0f,1.0f,1.0f));

float Vector_angleBetween(CVector3 a, CVector3 b);

bool Line_getIntercept(CVector2& i, CLine2D a, CLine2D b);
bool Line_getIntercept(CVector3& i, CLine3D a, CLine3D b);
bool Line_getIntercept(CVector3& i, CLine3D l, C3DPlane p);

float Plane_getDistanceFromPoint(C3DPlane plane,CVector3 point, bool* behind_plane = nullptr);
//Allows you to check whether the point is within a set of bounds. If not returns FLT_MAX. 
//Padding increases the bounds by the amount set if you want to add some wiggle room ;).
float Plane_getDistanceFromPoint(C3DPlane plane, CVector3 point, CVector3 min, CVector3 max, float min_padding = 0.0f, float max_padding = 0.0f);

CVector3 Plane_projectPoint(C3DPlane plane, CVector3 point);

bool Collision_AABBCheck(CVector3 min, CVector3 max, CVector3 point);
bool Collision_AABBCheck(CVector3 min_1, CVector3 max_1, CVector3 min_2, CVector3 max_2);

bool Collision_SphereAABBCheck(CVector3 center, float radius, CVector3 _min, CVector3 _max);
bool Collision_SphereCheck(CVector3 center, float radius, CVector3 point);

//C3DPlane must have a size of 6
bool Collision_TriAABBCheck(C3DPlane tri, C3DPlane* col_planes);
bool Collision_TriAABBCheck(C3DPlane tri, CVector3 min, CVector3 max);
bool Collision_TriSphereCheck(C3DPlane tri, CVector3 center, float radius);
bool Collision_TriCheck(C3DPlane tri, C3DPlane tri2);

CVector3 Transform_lerp(CVector3 to, CVector3 from, float factor);
CVector2 Transform_lerp(CVector2 to, CVector2 from, float factor);
