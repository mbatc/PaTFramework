#pragma once

#include <string>
#include "CMeshData.h"

#include "CImporterBase.h"

class CMeshImporter : public CImporterBase
{
public:
	CMeshImporter(std::string file);

	//This function is only valid once as the datas pointers are set to null
	//to prevent the data from being destroyed when the importer gets destroyed
	//Call reimport() if the importer still needs to be used after get_data() call
	CMeshData*		get_data(unsigned int index);
	unsigned int	get_mesh_count();

	void clean();
private:
	void zero();
	unsigned int	do_import();
	unsigned int	load_material_data(int index);
	unsigned int	load_mesh_data();

	std::vector<MeshMaterialData> m_matdata;

	CMeshData**		m_data;
	int				m_nData;

};