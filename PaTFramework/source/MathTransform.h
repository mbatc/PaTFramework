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

bool Line_getIntercept(CVector2& i, CLine2D a, CLine2D b);
bool Line_getIntercept(CVector3& i, CLine3D a, CLine3D b);
bool Line_getIntercept(CVector3& i, CLine3D l, C3DPlane p);

float Plane_getDistanceFromPoint(C3DPlane plane,CVector3 point);

bool Collision_AABBCheck(CVector3 min, CVector3 max, CVector3 point);
bool Collision_AABBCheck(CVector3 min_1, CVector3 max_1, CVector3 min_2, CVector3 max_2);

bool Collision_SphereAABBCheck(CVector3 center, float radius, CVector3 _min, CVector3 _max);
bool Collision_SphereCheck(CVector3 center, float radius, CVector3 point);

CVector3 Transform_lerp(CVector3 to, CVector3 from, float factor);
CVector2 Transform_lerp(CVector2 to, CVector2 from, float factor);
