#pragma once

#include "CD3DGraphics.h"

class CMeshImporter;


struct MeshMaterialData
{
	int id;
	std::string mat_name;

	std::string _diffuse;
	std::string _bump;
	std::string _specular;

	bool has_diffuse;
	bool has_bump;
	bool has_specular;
};

class CMeshData
{
	friend CMeshImporter;
public:
	CMeshData();
	~CMeshData();

	//Resizes the vertex buffer to the specified size, copies the old data into the new buffer
	//If the new size is smaller to will truncate the data that does not fit into the buffer
	void resize_vertex_buffer(int size);
	void resize_index_buffer(int size);
	//Adds the vertex passed to the function
	unsigned int add_vert(CUSTOMVERTEX vert);
	//Adds a vertex with the default values
	unsigned int add_vert();
	//Returns a reference to the vertex at the specified index
	CUSTOMVERTEX&	get_vert(unsigned int i);
	CUSTOMVERTEX*	get_vert_data();
	int				get_vertcount() const;

	unsigned int	add_index(unsigned long index = 0);
	unsigned long&	get_index(unsigned int i);
	unsigned long*	get_index_data();
	int				get_indexcount() const;

	void				set_material(MeshMaterialData mat) { m_material = mat; }
	const MeshMaterialData	get_materialInfo() const { return m_material; }
	std::string		m_name;
private:
	CUSTOMVERTEX*	m_pVertexBuffer;
	int				m_vertex_count;

	unsigned long*	m_pIndexBuffer;
	int				m_index_count;

	MeshMaterialData m_material;
};