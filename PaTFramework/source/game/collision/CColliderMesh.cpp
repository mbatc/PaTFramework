#include "CColliderMesh.h"
#include "../../CResourceBank.h"

int CColliderMesh::_derived_id;

CColliderMesh::CColliderMesh()
{

}

CColliderMesh::~CColliderMesh()
{

}

unsigned int CColliderMesh::update()
{
	for (size_t i = 0; i < m_collision_face.size(); i++)
	{
		m_collision_face[i].get_transform() = get_transform();
		m_collision_face[i].update();
	}
	return CColliderBase::update();
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

std::vector<CColliderMesh::STRI> CColliderMesh::separate_mesh(CMeshData& meshData)
{
	std::vector<STRI> tris(meshData.get_indexcount()/3);
	for (size_t i = 0; i < tris.size(); i++)
	{
		tris[i].norm = -meshData.get_vert(meshData.get_index(i * 3)).normal;
		tris[i].norm.x = -tris[i].norm.x;
		tris[i].norm.y = tris[i].norm.y;

		for (int vert = 0; vert < 3; vert++)
		{
			tris[i].pos[vert] = -meshData.get_vert(
				meshData.get_index(i * 3 + vert)).position;
			tris[i].pos[vert].x = -tris[i].pos[vert].x;
			//tris[i].pos[vert].y = -tris[i].pos[vert].y;
		}
	}
	return tris;
}

void CColliderMesh::add_triangles(std::vector<STRI> tris)
{
	for (size_t t_i = 0; t_i < tris.size(); t_i++)
	{
		CColliderTriangle col;
		col.m_group_name = m_group_name;
		col.init(tris[t_i].pos[0], tris[t_i].pos[1], tris[t_i].pos[2]);
		float angle = Vector_angleBetween(col.get_closestside(CVector3()).get_normal(), tris[t_i].norm);
		if(M_PI_2 < angle && M_PI_2*3 > angle)
			col.init(tris[t_i].pos[2], tris[t_i].pos[1], tris[t_i].pos[0]);
		m_collision_face.push_back(col);
	}
}

std::vector<CCollisionData> CColliderMesh::get_data()
{
	int data_count = get_collisioncount();
	std::vector<CCollisionData> data;

	for (int i = 0; i < data_count; i++)
		data.push_back(get_collisiondata(i));

	return data;
}