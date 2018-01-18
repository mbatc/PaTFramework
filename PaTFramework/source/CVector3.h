#pragma once


#ifdef _WIN32 || _WIN64
	#include "CD3DGraphics.h"
#endif

class CVector3
{
public:
#ifdef _WIN32 || _WIN64
	CVector3(D3DXVECTOR3 v);
#endif
	CVector3();
	CVector3(float x, float y, float z);
	~CVector3();

	//0 == x; 1 == y; 2 == z;
	float get(int axis);
	
	float x,y,z;

	float Magnitude();
	float dot(CVector3 v) {
		return x*v.x + y*v.y + z*v.z;
	}

	CVector3 operator +  (CVector3 in)
	{
		CVector3 out;
		out.x = this->x + in.x;
		out.y = this->y + in.y;
		out.z = this->z + in.z;

		return out;
	}

	CVector3 operator +  (float in)
	{
		CVector3 out;
		out.x = this->x + in;
		out.y = this->y + in;
		out.z = this->z + in;

		return out;
	}

	CVector3 operator - (CVector3 in)
	{
		CVector3 out;
		out.x = this->x - in.x;
		out.y = this->y - in.y;
		out.z = this->z - in.z;

		return out;
	}

	CVector3 operator += (CVector3 in)
	{
		this->x += in.x;
		this->y += in.y;
		this->z += in.z;

		return *this;
	}
	CVector3 operator += (float in)
	{
		this->x += in;
		this->y += in;
		this->z += in;

		return *this;
	}


	CVector3 operator * (float in)
	{
		CVector3 out;
		out.x = this->x * in;
		out.y = this->y * in;
		out.z = this->z * in;

		return out;
	}

	CVector3 operator * (CVector3 in)
	{
		CVector3 out;
		out.x = this->x * in.x;
		out.y = this->y * in.y;
		out.z = this->z * in.z;

		return out;
	}

	CVector3 operator *= (CVector3 in)
	{
		this->x *= in.x;
		this->y *= in.y;
		this->z *= in.z;

		return *this;
	}

	CVector3 operator / (float in)
	{
		CVector3 out;
		out.x = this->x / in;
		out.y = this->y / in;
		out.z = this->z / in;

		return out;
	}

	bool operator == (CVector3 in)
	{
		return this->x == in.x && this->y == in.y && this->z == in.z;
	}
};