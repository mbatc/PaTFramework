#include "CMeshImporter.h"
#include "helpers\MemoryHelper.h"
#include "CIO.h"

#include "CLog.h"

CMeshImporter::CMeshImporter(std::string file)
	:
	CImporterBase(file)
{
	zero();
}

CMeshData * CMeshImporter::get_data(unsigned int index)
{
	if (index < 0 || index > m_nData)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Invalid mesh index passed to CMeshImporter::get_data(): %d", index);
		return NULL;
	}

	CMeshData* ret_data = new CMeshData;
	*ret_data = *m_data[index];

	m_data[index]->m_pVertexBuffer = 0;
	m_data[index]->m_vertex_count = 0;
	m_data[index]->m_index_count = 0;
	m_data[index]->m_pIndexBuffer = 0;

	return ret_data;
}

unsigned int CMeshImporter::get_mesh_count()
{
	return m_nData;
}

void CMeshImporter::clean()
{
	for (int i = 0; i < m_nData; i++)
	{
		MEMORY::SafeDelete(m_data[i]);
	}
	MEMORY::SafeDeleteArr(m_data);
	m_matdata.clear();
}

void CMeshImporter::zero()
{
	m_data = 0;
	m_nData = 0;
}

unsigned int CMeshImporter::do_import()
{
	unsigned int mesh_count = read_int();
	unsigned int mat_count = read_int();

	m_matdata.resize(mat_count);

	for (int i = 0; i < mat_count; i++)
	{
		load_material_data(i);
	}

	for (int i = 0; i < mesh_count; i++)
	{
		load_mesh_data();
	}

	return getlasterror();
}

unsigned int CMeshImporter::load_material_data(int index)
{
	//Load material index
	m_matdata[index].id = read_int();
	m_matdata[index].mat_name = m_matdata[index].id;

	//Scan diffuse
	int nTex = read_int();
	for (int i = 0; i < nTex; i++)
	{
		m_matdata[index]._diffuse = read_string();
		m_matdata[index].has_diffuse = strcmp(m_matdata[index]._diffuse.c_str(),
			"0");

		if (m_matdata[index].has_diffuse)
			m_matdata[index]._diffuse = m_directory + m_matdata[index]._diffuse;
	}

	//Scan specular
	nTex = read_int();
	for (int i = 0; i < nTex; i++)
	{
		m_matdata[index]._specular = read_string();
		m_matdata[index].has_specular = strcmp(m_matdata[index]._specular.c_str(),
			"0");

		if (m_matdata[index].has_specular)
			m_matdata[index]._specular = m_directory + m_matdata[index]._specular;
	}
	//Scan bump
	nTex = read_int();
	for (int i = 0; i < nTex; i++)
	{
		m_matdata[index]._bump = read_string();
		m_matdata[index].has_bump = strcmp(m_matdata[index]._bump.c_str(),
			"0");

		if (m_matdata[index].has_bump)
			m_matdata[index]._bump = m_directory + m_matdata[index]._bump;
	}

	return 0;
}

unsigned int CMeshImporter::load_mesh_data()
{
	MEMORY::LIST::AddToListArr(m_data, new CMeshData(), m_nData);
	int i = m_nData - 1;

	m_data[i]->m_name = read_string();
	unsigned int mat_id = read_int();
	unsigned int vert_count = read_int();
	unsigned int index_count = read_int();


	m_data[i]->resize_vertex_buffer(vert_count);
	m_data[i]->resize_index_buffer(index_count);
	
	float* pos = new float[vert_count * 3];
	float* norm = new float[vert_count * 3];
	float* UV = new float[vert_count * 2];
	unsigned long* indices = new unsigned long[index_count];

	_io.ReadF(m_file, pos, sizeof(float), vert_count*3);
	_io.ReadF(m_file, norm, sizeof(float), vert_count*3);
	_io.ReadF(m_file, UV, sizeof(float), vert_count*2);

	_io.ReadF(m_file, indices, sizeof(unsigned long), index_count);

	for (int j = 0; j < vert_count; j++)
	{
		m_data[i]->get_vert(j).position = D3DXVECTOR3(pos[j*3], pos[(j * 3) + 1], pos[(j * 3) + 2]);
		m_data[i]->get_vert(j).normal = D3DXVECTOR3(norm[j*3], norm[(j * 3) + 1], norm[(j * 3) + 2]);
		m_data[i]->get_vert(j).UV = D3DXVECTOR2(UV[j*2], 1.0f-UV[(j*2) + 1]);
		m_data[i]->get_vert(j).color = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
	}

	for (int j = 0; j < index_count; j++)
	{
		m_data[i]->get_index(j) = indices[j];
	}

	for (int j = 0; j < m_matdata.size(); j++)
	{
		if (m_matdata[i].id == mat_id)
			m_data[i]->set_material(m_matdata[i]);
	}

	return 0;
}
