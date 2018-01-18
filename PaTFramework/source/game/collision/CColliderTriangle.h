#pragma once

#include "CColliderBase.h"

#include "../../C3DPlane.h"

derive_ColliderType(CColliderTriangle)
{
public:
	CColliderTriangle() : CColliderBase_inherit() {}
	~CColliderTriangle() {}

	void init(CVector3 p1, CVector3 p2, CVector3 p3);

	C3DPlane get_closestside(CVector3 to) override;
	CVector3 get_vertex_transformed(int index);
private:
	void update_plane();

	CVector3 m_points[3];
	C3DPlane m_plane;
};