#include "CDXMesh.h"
#include "CD3DGraphics.h"
#include "CLog.h"

CDXMesh::CDXMesh(CMeshData * mesh_data)
	:
	m_data(mesh_data)
{}

CDXMesh::~CDXMesh()
{}

unsigned int CDXMesh::init(CD3DGraphics * gfx)
{
	gfx->CreateVertexBuffer(m_data->get_vert_data(), m_data->get_vertcount() , &m_pVB);
	gfx->CreateIndexBuffer(m_data->get_index_data(), m_data->get_indexcount(), &m_pIB);

	if (!m_pVB)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to create vertex buffer from CMeshData(ptr=%p)", m_data);
		return 1;
	}
	if (!m_pIB)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to create index buffer from CMeshData(ptr=%p)", m_data);
		return 2;
	}

	m_gfx = gfx;
	return 0;
}

void CDXMesh::release()
{
	m_gfx->SafeRelease(m_pVB);
	m_gfx->SafeRelease(m_pIB);
}

unsigned int CDXMesh::get_vertexbuffer(ID3D11Buffer ** buffer)
{
	*buffer = m_pVB;
	m_pVB->AddRef();

	return 0;
}

unsigned int CDXMesh::get_indexbuffer(ID3D11Buffer ** buffer)
{
	*buffer = m_pIB;
	m_pIB->AddRef();

	return 0;
}
