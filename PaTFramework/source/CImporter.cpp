#include "CImporter.h"

#include "CResourceBank.h"

#include "CMeshImporter.h"
#include "CTexture.h"

#include "CIO.h"

#include "CApp.h"

#include "helpers//FileHelper.h"
#include "helpers//StringHelper.h"

CImporter* CImporter::_instance = NULL;

CImporter::~CImporter()
{
}

unsigned int CImporter::import(std::string file)
{
	unsigned int result = IMPORT_NO_ERR;

	CResourceBank* bank = CResourceBank::get_instance();
	if (bank->isFileLoaded(file))
	{
		if (!_force_import)
			return result;

		bank->rem_file(file);
	}
	
	char* filename = 0;
	STRING::Copy(filename, file.c_str());
	char* ext = FILEH::GetFileExtension(filename);

	for (int i = 0; i < m_image_type.size(); i++)
	{
		if (!strcmp(ext, m_image_type[i].c_str()))
			return import_texture(file);
	}

	_io.OpenFile(file, "rb");

	int len = 0;
	_io.ReadF(file, &len, sizeof(int), 1);
	char* buffer = new char[len + 1];
	_io.ReadF(file, buffer, sizeof(char), len);
	buffer[len] = 0;

	if (!strcmp(ATD_TYPE_MESH, buffer))
	{
		result = import_mesh(file);
	}
	else if (!strcmp(ATD_TYPE_AUDIO, buffer))
	{
		result = import_audio(file);
	}
	else if (!strcmp(ATD_TYPE_SCENE, buffer))
	{
		result = import_scene(file);
	}
	else if (!strcmp(ATD_TYPE_GMOBJ, buffer))
	{
		result = import_gameobject(file);
	}
	else
	{
		result = IMPORT_ERR_INVALID_FILE_HEADER;
	}
	_io.CloseFile(file);
	return result;
}

unsigned int CImporter::import_mesh(std::string file)
{
	CMeshImporter importer(file);
	importer.import(file);

	CResourceBank* res = CResourceBank::get_instance();
	for (int i = 0; i < importer.get_mesh_count(); i++)
	{
		CMeshData* data = importer.get_data(i);
		res->add_mesh(data, true, data->m_name, file);

		MeshMaterialData mat = data->get_materialInfo();
		
		if (mat.has_bump)
			import(mat._bump);
		if (mat.has_diffuse)
			import(mat._diffuse);
		if (mat.has_specular)
			import(mat._specular);
	}

	importer.clean();

	//IF DIRECTX IS BEING USED ADD A DXMESH AS WELL
	res->create_dxmesh_from_file(file);
	int count = res->get_dxmesh_list().get_resource_count_in_file(file);
	for (int i = 0; i < count; i++)
	{
		CDXMesh* mesh = res->get_dxmesh_list().get_resource_from_file_at_index(file, i);
		if (mesh)
			mesh->init(CAppBridge::get_instance()->get_sysGfx());
	}

	return importer.getlasterror();
}

unsigned int CImporter::import_texture(std::string file)
{
	CSurface image;
	image.Init(file.c_str());
	CTexture* tex = new CTexture;

	if (!tex->InitTexture(image, *CAppBridge::get_instance()->get_sysGfx()))
		return 0;

	CResourceBank* res = CResourceBank::get_instance();
	res->add_dxtexture(tex, true, file, file);
	return 0;
}

unsigned int CImporter::import_audio(std::string file)
{
	return 0;
}

unsigned int CImporter::import_scene(std::string file)
{
	return 0;
}

unsigned int CImporter::import_gameobject(std::string file)
{
	return 0;
}

void CImporter::init_image_types()
{
	m_image_type.push_back(".jpg");
	m_image_type.push_back(".png");
	m_image_type.push_back(".tif");
	m_image_type.push_back(".bmp");
}
