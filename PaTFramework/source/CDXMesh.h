#pragma once

#include "CMeshData.h"

class CD3DGraphics;

class CDXMesh
{
public:
	CDXMesh(CMeshData* mesh_data);
	~CDXMesh();

	const CMeshData* get_data() { return m_data; }

	unsigned int	init(CD3DGraphics* gfx);
	void			release();

	unsigned int get_vertexbuffer(ID3D11Buffer** buffer);
	unsigned int get_indexbuffer(ID3D11Buffer** buffer);
private:
	ID3D11Buffer* m_pVB;
	ID3D11Buffer* m_pIB;

	CMeshData*		m_data;
	CD3DGraphics*	m_gfx;

	std::vector<ID3D11Texture2D*> m_resources;
};