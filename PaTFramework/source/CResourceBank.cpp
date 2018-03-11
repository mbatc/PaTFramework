#include "CResourceBank.h"
#include "CLog.h"

#include <stdlib.h>

CResourceBank* CResourceBank::_instance = nullptr;

CResourceBank::CResourceBank()
{
}

CResourceBank::~CResourceBank()
{
	shutdown();
}

////////////////////////////////////////////////////////
//MESH DATA

int CResourceBank::add_mesh(CMeshData * data, bool allow_delete, std::string name, std::string filename)
{
	unsigned int id = m_mesh.add_resource(data, allow_delete, get_unique_id(), name, filename);
	AddResIDToFileDef(filename, id);
	return id;
}

int CResourceBank::create_mesh()
{
	return add_mesh(new CMeshData, true);
}

void CResourceBank::rem_mesh_at_index(int index)
{
	m_mesh.rem_resource_at_index(index);
}

void CResourceBank::rem_mesh(std::string name)
{
	m_mesh.rem_resource(name);
}

void CResourceBank::rem_mesh(unsigned int id)
{
	m_mesh.rem_resource(id);
}

void CResourceBank::rem_mesh(CMeshData * data)
{
	m_mesh.rem_resource(data);
}

CMeshData * CResourceBank::get_mesh(unsigned int id)
{
	return m_mesh.get_resource(id);
}

CMeshData * CResourceBank::get_mesh(std::string name)
{
	return m_mesh.get_resource(name);
}

CMeshData * CResourceBank::get_mesh_at_index(int index)
{
	return m_mesh.get_resource_at_index(index);
}

///////////////////////////////////////////////////////////////
//DIRECTX MESH

int CResourceBank::add_dxmesh(CDXMesh* mesh, bool allow_delete,  std::string name)
{
	unsigned int id = m_dxMesh.add_resource(mesh, allow_delete, get_unique_id(), name);
	AddResIDToFileDef("(null)", id);
	return id;
}

int CResourceBank::create_dxmesh(CMeshData * data, bool allow_delete, std::string name, std::string filename)
{
	unsigned int id = m_dxMesh.add_resource(new CDXMesh(data), allow_delete, get_unique_id(), name, filename);
	AddResIDToFileDef(filename, id);
	return id;
}

int CResourceBank::create_dxmesh(unsigned int id)
{
	CMeshData* data = m_mesh.get_resource_at_index(id);
	return create_dxmesh(m_mesh.get_resource(id), true, m_mesh.get_resource_name(id), m_mesh.get_resource_filename(id));
}

int CResourceBank::create_dxmesh_at_index(int index)
{
	return create_dxmesh(m_mesh.get_resource_at_index(index), true, m_mesh.get_resource_name_at_index(index), m_mesh.get_resource_filename_at_index(index));
}

int CResourceBank::create_dxmesh(std::string name)
{
	CMeshData* data = m_mesh.get_resource(name);
	return create_dxmesh(data, true, name);
}

std::vector<int> CResourceBank::create_dxmesh_from_file(std::string filename)
{
	std::vector<int> indices;

	int count = m_mesh.get_resource_count_in_file(filename);
	indices.resize(count);

	for (int i = 0; i < count; i++)
	{
		CMeshData* data = m_mesh.get_resource_from_file_at_index(filename, i);
		indices[i] = create_dxmesh(data, true, m_mesh.get_resource_name(data), filename);
	}

	return indices;
}

void CResourceBank::rem_dxmesh_at_index(int index)
{
	m_dxMesh.get_resource_at_index(index)->release();
	m_dxMesh.rem_resource_at_index(index);
}

void CResourceBank::rem_dxmesh(CMeshData * data)
{
	for (int i = 0; i < m_dxMesh.get_count(); i++)
	{
		if (m_dxMesh.get_resource_at_index(i)->get_data() == data) {
			if(m_dxMesh.allowdelete(i))
				m_dxMesh.get_resource_at_index(i)->release();

			m_dxMesh.rem_resource(i);
			return;
		}
	}
}

void CResourceBank::rem_dxmesh(CDXMesh * mesh)
{
	if (m_dxMesh.allowdelete(mesh))
		mesh->release();
	m_dxMesh.rem_resource(mesh);
}

void CResourceBank::rem_dxmesh(unsigned int id)
{
	if (m_dxMesh.allowdelete_id(id))
		m_dxMesh.get_resource(id)->release();
	m_dxMesh.rem_resource(id);
}

CDXMesh * CResourceBank::get_dxmesh_at_index(int index)
{
	return m_dxMesh.get_resource_at_index(index);
}

CDXMesh * CResourceBank::get_dxmesh(CMeshData * data)
{
	for (int i = 0; i < m_dxMesh.get_count(); i++)
	{
		if (m_dxMesh.get_resource_at_index(i)->get_data() == data)
			return m_dxMesh.get_resource_at_index(i);
	}
	return NULL;
}

CDXMesh * CResourceBank::get_dxmesh(unsigned int id)
{
	return m_dxMesh.get_resource(id);
}

CDXMesh * CResourceBank::get_dxmesh(std::string name)
{
	return m_dxMesh.get_resource(name);
}

////////////////////////////////////////////////////
//CLEANUP FUNCTIONS

int CResourceBank::add_dxtexture(CTexture * tex, bool allow_delete, std::string name, std::string filename)
{
	unsigned int id = m_dxTexture.add_resource(tex, allow_delete, get_unique_id(), name, filename);
	AddResIDToFileDef(filename, id);
	return id;
}

void CResourceBank::rem_file(std::string file)
{
	SResourceFile file_def;
	int file_index = -1;
	for (int i = 0; i < m_resource_file.size(); i++)
	{
		if (m_resource_file[i]._filename == file)
		{
			file_def = m_resource_file[i];
			file_index = i;
		}
	}

	if (file_index < 0)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING,
			"Unable to remove file (%s) from resource bank: Does not exist",
			file.c_str());
		return;
	}

	for (int i = 0; i < file_def._resource_id.size(); i++)
	{
		unsigned int id = file_def._resource_id[i];
		switch (get_resource_type(id))
		{
		case RESBANK_TYPE_MESH:
			m_mesh.rem_resource(id);
			break;
		case RESBANK_TYPE_DXMESH:
			m_dxMesh.rem_resource(id);
			break;
		case RESBANK_TYPE_DXTEX:
			break;
		default:
			Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to remove resource (FILE: %s, ID:%d): does not exist", file.c_str(), id);
			break;
		}
	}

	m_resource_file.erase(m_resource_file.begin() + file_index);
	return;
}

bool CResourceBank::isFileLoaded(std::string file)
{
	for (int i = 0; i < m_resource_file.size(); i++)
	{
		if (m_resource_file[i]._filename == file)
			return true;
	}
	return false;
}

unsigned int CResourceBank::get_resource_type(unsigned int ID)
{
	void* res = m_mesh.get_resource(ID);
	if (res)
		return RESBANK_TYPE_MESH;

	res = m_dxMesh.get_resource(ID);
	if (res)
		return RESBANK_TYPE_DXMESH;

	res = m_dxTexture.get_resource(ID);
	if (res)
		return RESBANK_TYPE_DXTEX;

	return RESBANK_TYPE_UNKNOWN;
}

void CResourceBank::shutdown()
{
	//Release all resources

	clean_mesh();
	clean_dxmesh();
	clean_dxtexture();
}

void CResourceBank::AddResIDToFileDef(std::string file, unsigned int ID)
{
	//Find the specified file and add the resource ID to the definition
	for (int i = 0; i < m_resource_file.size(); i++)
	{
		if (m_resource_file[i]._filename == file)
		{
			m_resource_file[i]._resource_id.push_back(ID);
		}
	}

	//If the file was not found add a new file definition
	SResourceFile res_file;
	res_file._filename = file;
	res_file._resource_id.push_back(ID);

	m_resource_file.push_back(res_file);
}

unsigned int CResourceBank::get_unique_id()
{
	unsigned int id = rand()*(INT_MAX-1)+1;

	if (get_resource_type(id) != RESBANK_TYPE_UNKNOWN)
		return get_unique_id();

	return id;
}

void CResourceBank::clean_mesh()
{
	m_mesh.cleanup();
}

void CResourceBank::clean_dxmesh()
{
	for (int i = m_mesh.get_count(); i >= 0; i--)
	{
		if (m_dxMesh.allowdelete(i))
			m_dxMesh.get_resource_at_index(i)->release();
	}
	m_dxMesh.cleanup();
}

void CResourceBank::clean_dxtexture()
{
	m_dxTexture.cleanup();
}
