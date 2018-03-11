#include "CColliderMesh.h"
#include "../../CResourceBank.h"

int CColliderMesh::_derived_id;

CColliderMesh::CColliderMesh()
{

}

CColliderMesh::~CColliderMesh()
{

}

void CColliderMesh::load_mesh(std::string name)
{
	//Clear previous data
	m_collision_face.clear();

	CResourceBank* _res = CResourceBank::get_instance();
	auto mesh_list = _res->get_mesh_list();
	if (mesh_list.is_file(name))
	{
		//Get each mesh in the file and load the data
		int mesh_count = mesh_list.get_resource_count_in_file(name);
		for (int mesh_index = 0; mesh_index < mesh_count; mesh_index++)
		{
			CMeshData* mesh = mesh_list.get_resource_from_file_at_index(name,
				mesh_index);
			add_triangles(separate_mesh(*mesh));
		}
	}
	else
	{
		//Only a single mesh will be loaded
		CMeshData* mesh = mesh_list.get_resource(name);
		add_triangles(separate_mesh(*mesh));
	}
}

unsigned int CColliderMesh::get_collisioncount() 
{
	unsigned int count = 0;
	for (size_t i = 0; i < m_collision_face.size(); i++)
	{
		count += m_collision_face[i].get_collisioncount();
	}

	return count;
}

CCollisionData CColliderMesh::get_collisiondata(int index)
{
	for (size_t i = 0; i < m_collision_face.size(); i++)
	{
		if (index >= m_collision_face[i].get_collisioncount()) {
			index -= m_collision_face[i].get_collisioncount();
		}
		else {
			return m_collision_face[i].get_collisiondata(index);
		}
	}
	return CCollisionData();
}

CColliderTriangle* CColliderMesh::get_face(int index)
{
	if (index < 0 || index >= m_collision_face.size())
		return nullptr;
	return &m_collision_face[index];
}

const int CColliderMesh::get_face_count()
{
	return m_collision_face.size();
}

std::vector<std::vector<CVector3>> CColliderMesh::separate_mesh(CMeshData& meshData)
{
	std::vector<std::vector<CVector3>> tris(meshData.get_indexcount()/3);
	for (size_t i = 0; i < tris.size(); i++)
	{
		for (int vert = 0; vert < 3; vert++)
			tris[i].push_back(meshData.get_vert(
				meshData.get_index(i * 3 + vert)).position);
	}
	return tris;
}

void CColliderMesh::add_triangles(std::vector<std::vector<CVector3>> tris)
{
	for (size_t t_i = 0; t_i < tris.size(); t_i++)
	{
		if (tris[t_i].size() < 3)
			continue;

		CColliderTriangle col;
		col.init(tris[t_i][0], tris[t_i][0], tris[t_i][0]);
		m_collision_face.push_back(col);
	}
}