#pragma once

#include "CColliderBase.h"
#include "CColliderTriangle.h"

#include "../../CMeshData.h"

#include <vector>
#include "../../Vectors.h"

derive_ColliderType(CColliderMesh)
{
public:
	CColliderMesh();
	~CColliderMesh();

	void load_mesh(std::string name);

	unsigned int get_collisioncount() override;
	CCollisionData get_collisiondata(int index) override;

	CColliderTriangle*	get_face(int index);
	const int			get_face_count();
private:
	std::vector<std::vector<CVector3>> separate_mesh(CMeshData& meshData);
	void add_triangles(std::vector<std::vector<CVector3>> tris);

	std::vector<CColliderTriangle>	m_collision_face;
};
