#pragma once

#include "CColliderBase.h"

#define AABB_VERT_COUNT 8
#define AABB_SIDE_COUNT 6

derive_ColliderType(CColliderAABB)
{
public:
	CColliderAABB();
	~CColliderAABB();

	void set_bounds(CVector3 min, float width, float height, float depth);
	void set_bounds(CVector3 min, CVector3 max);

	C3DPlane		get_closestside(CVector3 to) override; 
	C3DPlane		get_closestside(C3DPlane to);

	CVector3 get_boundmin() { return m_min+get_transform().get_translation(); }
	CVector3 get_boundmax() { return m_max+get_transform().get_translation(); }

	CVector3 get_collision_point(CColliderBase* base) override;
	CVector3 get_vertex(int index) const { if (index < 0 || index >= AABB_VERT_COUNT) return CVector3(); return points[index]; }
	CVector3 get_vertex_translated(int index) { return get_vertex(index) + get_transform().get_translation(); }

	C3DPlane* get_sides() { return sides; }
	C3DPlane* get_sides_translated() { update_sides_trans(); return sides_translated; }
private:
	void update_sides();
	void update_sides_trans();

	CVector3 m_min;
	CVector3 m_max;

	CVector3 points[AABB_VERT_COUNT];
	C3DPlane sides[AABB_SIDE_COUNT];

	C3DPlane sides_translated[AABB_SIDE_COUNT];
};