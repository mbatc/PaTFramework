#pragma once

#include "../CVector3.h"

class CTransform
{
public:
	CTransform() { set_scale(CVector3(1.0f, 1.0f, 1.0f)); }
	~CTransform() {}

	void translate(CVector3 v) { m_trans += v; }
	void rotate(CVector3 v) { m_rot += v; }
	void scale(CVector3 v) { m_scl *= v; }

	void set_translation(CVector3 t) { m_trans = t; }
	void set_rotation(CVector3 r) { m_rot = r; }
	void set_scale(CVector3 s) { m_scl = s; }

	CVector3& get_translation() { return m_trans; }
	CVector3& get_rotation() { return m_rot; }
	CVector3& get_scale() { return m_scl; }
private:

	CVector3 m_trans;
	CVector3 m_rot;
	CVector3 m_scl;
};