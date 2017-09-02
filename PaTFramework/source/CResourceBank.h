#pragma once

#include "helpers\MemoryHelper.h"

#include "CResourceList.h"

#include "CMeshData.h"
#include "CDXMesh.h"
#include "CTexture.h"

#include "CLog.h"

#include <vector>
#include <string>

#define RESBANK_TYPE_UNKNOWN	0x00
#define RESBANK_TYPE_MESH		0x01
#define RESBANK_TYPE_DXMESH		0x02
#define RESBANK_TYPE_DXTEX		0x03


class CResourceBank
{
	struct SResourceFile
	{
		std::string _filename;
		std::vector<unsigned int> _resource_id;
	};

private:
	CResourceBank();
	static CResourceBank* _instance;
public:
	static CResourceBank* get_instance() {
		if (!_instance)
		{
			_instance = new CResourceBank();
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CResourceBank instance created (ptr=%p)", _instance);
		}
		return _instance;
	}
	static void destroy() {
		if (_instance)
		{
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CResourceBank instance destroyed (ptr=%p)", _instance);
			delete _instance;
		}
		_instance = 0;
	}
	~CResourceBank();

	////////////////////////////////////////////////////////
	//MESH DATA RESOURCE
	int			add_mesh(CMeshData* data, bool allow_delete, 
		 std::string name = "(null)", std::string filename = "(null)");
	int			create_mesh();

	void		rem_mesh_at_index(int index);
	void		rem_mesh(std::string name);
	void		rem_mesh(unsigned int id);
	void		rem_mesh(CMeshData* data);

	CMeshData*	get_mesh(std::string filename, std::string name);
	CMeshData*	get_mesh(std::string filename, int index);

	CMeshData*	get_mesh(unsigned int id);
	CMeshData*	get_mesh(std::string name);
	CMeshData*	get_mesh_at_index(int index);

	CResourceList<CMeshData>& get_mesh_list() { return m_mesh; }
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	//DIRECTX MESH DATA RESOURCE
	//These functions initialise DirectX meshes from existing 
	//CMeshData objects that have been added to the resource
	//bank

	int			add_dxmesh(CDXMesh* mesh, bool allow_delete, std::string name = "(null)");

	//Create DXMesh from CMeshData passed to function
	//if the data passed has not been loaded into the resource bank, 
	//it will be added
	int			create_dxmesh(CMeshData* data, bool allow_delete, std::string name = "(null)", 
		std::string filename = "(null)");
	//Create DXMesh from CMeshData with the ID passed
	int			create_dxmesh(unsigned int id);
	//Create DXMesh from CMeshData at specified index
	int			create_dxmesh_at_index(int index);
	//Create DXMesh from CMeshData with the name passed 
	int			create_dxmesh(std::string name);

	//Create a DXMesh object for each mesh loaded from the file passed.
	std::vector<int> create_dxmesh_from_file(std::string filename);

	void		rem_dxmesh_at_index(int index);
	void		rem_dxmesh(CMeshData* data);
	void		rem_dxmesh(CDXMesh* mesh);
	void		rem_dxmesh(unsigned int id);

	CDXMesh*	get_dxmesh_at_index(int index);
	CDXMesh*	get_dxmesh(CMeshData* data);
	CDXMesh*	get_dxmesh(unsigned int id);
	CDXMesh*	get_dxmesh(std::string name);

	CResourceList<CDXMesh>& get_dxmesh_list() { return m_dxMesh; }
	////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////
	//DIRECTX TEXTURE RESOURCES

	int add_dxtexture(CTexture* tex, bool allow_delete = true, std::string name = "(null)", std::string filename = "(null)");

	CResourceList<CTexture>& get_dxtexture_list() { return m_dxTexture; }
	////////////////////////////////////////////////////////////
	
	void		rem_file(std::string file);
	bool isFileLoaded(std::string file);

	unsigned int get_resource_type(unsigned int ID);

	void shutdown();
private:
	void AddResIDToFileDef(std::string file, unsigned int ID);
	unsigned int get_unique_id();

	void clean_mesh();
	void clean_dxmesh();

	CResourceList<CMeshData>	m_mesh;
	CResourceList<CDXMesh>		m_dxMesh;
	CResourceList<CTexture>		m_dxTexture;

	std::vector<SResourceFile> m_resource_file;
};