#include "CVector3.h"

#include <cmath>

CVector3::CVector3(float x, float y, float z)
	:
	x(x),
	y(y),
	z(z)
{}


#ifdef _WIN32 || _WIN64
	CVector3::CVector3(D3DXVECTOR3 v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
#endif
CVector3::CVector3()
	:
	x(0.0f),
	y(0.0f),
	z(0.0f)
{}

CVector3::~CVector3()
{}

float CVector3::get(int axis)
{
	switch (axis)
	{
	case 0:
		return x;
	case 1:
		return y;
	case 2:
		return z;
	default:
		return _FMAX;
	};
}

float CVector3::Magnitude()
{
	float mag;
	
	mag = sqrt(x*x + y*y + z*z);

	return mag;
}