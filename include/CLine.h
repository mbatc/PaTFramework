#pragma once

#include "CVector2.h"
#include "CVector3.h"

template <class T> class CLine
{
public:
	CLine(T a, T b) {
		set_a(a);
		set_b(b);
	}

	virtual bool onXAxis() { return (m_dir.x == 1.0f && m_dir.y == 0.0f); }
	virtual bool onYAxis() { return (m_dir.x == 0.0f && m_dir.y == 1.0f); }

	void set_a(T a) {
		m_a = a;
		m_dir = direction();
	}

	void set_b(T b) {
		m_b = b;
		m_dir = direction();
	}

	float get_XatT(float t) {
		return m_a.x + m_dir.x*t;
	}
	float get_YatT(float t) {
		return m_a.y + m_dir.y*t;
	}

	float get_XatY(float y) {
		return get_XatT(get_TatY(y));
	}
	float get_YatX(float x) {
		return get_YatT(get_TatY(y));
	}

	virtual bool within_givenpoints(T p) {
		return	(p.x >= min(m_a.x, m_b.x) && p.x <= max(m_a.x, m_b.x)) &&
				(p.y >= min(m_a.y, m_b.y) && p.y <= max(m_a.y, m_b.y));
	}

	T get_dir() { return m_dir; }
	T get_a() { return m_a; }
	T get_b() { return m_b; }
protected:

	float get_TatX(float x) {
		if (m_dir.x == 0.0f)
			return 0.0f;
		return (x - m_a.x) / m_dir.x;
	}
	float get_TatY(float y) {
		if (m_dir.y == 0.0f)
			return 0.0f;
		return (y - m_a.y) / m_dir.y;
	}

	T direction() { return m_b - m_a; }

	T m_a;
	T m_b;
	T m_dir;
};

class CLine2D : public CLine <CVector2>
{
public:
	CLine2D(CVector2 a, CVector2 b):CLine(a,b) {}
};

class CLine3D : public CLine <CVector3>
{
public:
	CLine3D(CVector3 a, CVector3 b):CLine(a,b){}

	bool onXAxis() override { return CLine::onXAxis() && m_dir.z == 0.0f; }
	bool onYAxis() override { return CLine::onYAxis() && m_dir.z == 0.0f; }
	bool onZAxis() { return (m_dir.x == 0.0f && m_dir.y == 0.0f && m_dir.z == 0.0f); }

	float get_ZatT(float t) { return m_a.z + m_dir.z*t;	}
	float get_YatZ(float z) { return get_YatT(get_TatZ(z)); }
	float get_XatZ(float z) { return get_XatT(get_TatZ(z));	}
	float get_ZatX(float x) { return get_ZatT(get_TatX(x)); }
	float get_ZatY(float y) { return get_ZatT(get_TatY(y));	}

	bool within_givenpoints(CVector3 v) override {
		return v.z >= min(m_a.z,m_b.z) && v.z <= max(m_a.z, m_b.z) && CLine::within_givenpoints(v);
	}
private:
	float get_TatZ(float z) {
		if (m_dir.z == 0.0f)
			return 0.0f;
		return (z - m_a.z) / m_dir.z;
	}
};