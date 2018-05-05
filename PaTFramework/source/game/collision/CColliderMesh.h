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

	struct STRI {
		CVector3 pos[3];
		CVector3 norm;
	};

	unsigned int update() override;
	void load_mesh(std::string name);

	unsigned int get_collisioncount() override;
	CCollisionData get_collisiondata(int index) override;

	CColliderTriangle*	get_face(int index);
	const int			get_face_count();
protected:
	std::vector<CCollisionData> get_data() override;

private:
	std::vector<STRI> separate_mesh(CMeshData& meshData);
	void add_triangles(std::vector<STRI> tris);

	std::vector<CColliderTriangle>	m_collision_face;
};
