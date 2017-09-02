#include "CMeshData.h"
#include "helpers\MemoryHelper.h"

#include "CLog.h"

CMeshData::CMeshData()
	:
	m_pVertexBuffer(0),
	m_vertex_count(0),
	m_pIndexBuffer(0),
	m_index_count(0)
{}

CMeshData::~CMeshData()
{
	MEMORY::SafeDeleteArr(m_pVertexBuffer);
	MEMORY::SafeDeleteArr(m_pIndexBuffer);
	m_index_count = 0;
	m_vertex_count = 0;
}

void CMeshData::resize_vertex_buffer(int size)
{
	CUSTOMVERTEX* temp = new CUSTOMVERTEX[size];
	for (int i = 0; i < m_vertex_count && i < size; i++)
	{
		temp[i] = m_pVertexBuffer[i];
	}

	if (m_vertex_count < size)
	{
		for (int i = m_vertex_count; i < size; i++)
		{
			ZeroMemory(&temp[i], sizeof(CUSTOMVERTEX));
		}
	}

	MEMORY::SafeDeleteArr(m_pVertexBuffer);
	m_pVertexBuffer = temp;
	m_vertex_count = size;
}

void CMeshData::resize_index_buffer(int size)
{
	unsigned long* temp = new unsigned long[size];
	for (int i = 0; i < m_index_count && i < size; i++)
	{
		temp[i] = m_pIndexBuffer[i];
	}

	if (m_index_count < size)
	{
		for (int i = m_index_count; i < size; i++)
		{
			temp[i] = 0;
		}
	}

	MEMORY::SafeDeleteArr(m_pIndexBuffer);
	m_pIndexBuffer = temp;
	m_index_count = size;
}

unsigned int CMeshData::add_vert(CUSTOMVERTEX vert)
{
	MEMORY::LIST::AddToList(m_pVertexBuffer, &vert, m_vertex_count);
	return m_vertex_count - 1;
}

unsigned int CMeshData::add_vert()
{
	CUSTOMVERTEX vert;
	ZeroMemory(&vert, sizeof(CUSTOMVERTEX));

	MEMORY::LIST::AddToList(m_pVertexBuffer, &vert, m_vertex_count);
	return m_vertex_count - 1;
}

CUSTOMVERTEX & CMeshData::get_vert(unsigned int i)
{
	if (i < 0 || i >= m_vertex_count)
	{
		Log(this, DEBUGLOG_LEVEL_ERROR, "Invalid vertex index passed to CMeshData::get_vert() - returning first vert");
		return m_pVertexBuffer[0];
	}

	return m_pVertexBuffer[i];
}

CUSTOMVERTEX * CMeshData::get_vert_data()
{
	return m_pVertexBuffer;
}

int CMeshData::get_vertcount() const
{
	return m_vertex_count;
}

unsigned int CMeshData::add_index(unsigned long index)
{
	MEMORY::LIST::AddToList(m_pIndexBuffer, &index, m_index_count);
	return m_index_count - 1;
}

unsigned long & CMeshData::get_index(unsigned int i)
{
	if (i < 0 || i >= m_index_count)
	{
		Log(this, DEBUGLOG_LEVEL_ERROR, "Invalid index_buffer index passed to CMeshData::get_index() - returning first index");
		return m_pIndexBuffer[0];
	}

	return m_pIndexBuffer[i];
}

unsigned long * CMeshData::get_index_data()
{
	return m_pIndexBuffer;
}

int CMeshData::get_indexcount() const
{
	return m_index_count;
}
