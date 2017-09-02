#include "CSprite.h"
#include "helpers/Helper.h"

#define _USE_MATH_DEFINES
#include <math.h>

ID3D11Buffer* CSprite::m_pIndexBuffer = 0;
ID3D11Buffer* CSprite::m_pVertexBuffer = 0;

CSprite::CSprite()
	:
	m_pTexture(NULL),
	m_bInitFromSurface(false),/*
	m_pVertexBuffer(NULL),*/
	m_yPos(0),
	m_xPos(0),
	m_yScale(1.0f),
	m_xScale(1.0f),
	m_color(1.0f, 1.0f, 1.0f, 1.0f),
	m_anchorX(0.0f),
	m_anchorY(0.0f),
	m_bFlip(false),
	m_flag_dirty(true)
{
	m_areaLock.bottom = -1;
	m_areaLock.left = -1;
	m_areaLock.right = -1;
	m_areaLock.top = -1;
}

CSprite::~CSprite()
{

}

void CSprite::Init(char* filename, CD3DGraphics& gfx)
{
	if (!m_pIndexBuffer)
		InitIndexBuffer(gfx);
	if (!m_pVertexBuffer)
		InitVertexBuffer(gfx);

	//m_pTexture = SYSTEM::RESOURCE::GetTexture(filename);
	m_bInitFromSurface = false;

	CreateVertexBuffer(gfx);
}

void CSprite::InitVertexBuffer(CD3DGraphics gfx)
{
	MEMORY::DX::SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(CUSTOMVERTEX) * NVERTS_SPRITE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	CUSTOMVERTEX* temp = new CUSTOMVERTEX[NVERTS_SPRITE];
	temp[0].position = D3DXVECTOR3(-1.0f, -1.0f, 0.5f);
	temp[1].position = D3DXVECTOR3(1.0f, -1.0f, 0.5f);
	temp[2].position = D3DXVECTOR3(1.0f, 1.0f, 0.5f);
	temp[3].position = D3DXVECTOR3(-1.0f, 1.0f, 0.5f);

	temp[0].UV = D3DXVECTOR2(0.0f,0.0f); //TR
	temp[1].UV = D3DXVECTOR2(1.0f,0.0f); //TL
	temp[2].UV = D3DXVECTOR2(1.0f,1.0f); //BL
	temp[3].UV = D3DXVECTOR2(0.0f,1.0f); //BR

	temp[0].color = temp[1].color = temp[2].color = temp[3].color = D3DXVECTOR4(1.0f,1.0f,1.0f,1.0f);

	data.pSysMem = temp;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(gfx.Device()->CreateBuffer(&desc, &data, &m_pVertexBuffer)))
		return;
}

void CSprite::InitIndexBuffer(CD3DGraphics& gfx)
{
	D3D11_BUFFER_DESC indexBufferDesc;
	D3D11_SUBRESOURCE_DATA indexData;
	ZeroMemory(&indexBufferDesc, sizeof(D3D11_BUFFER_DESC));
	
	unsigned long* t_indexBuffer = new unsigned long[6];
	t_indexBuffer[0] = 0;
	t_indexBuffer[1] = 1;
	t_indexBuffer[2] = 2;
	t_indexBuffer[3] = 0;
	t_indexBuffer[4] = 2;
	t_indexBuffer[5] = 3;

	indexBufferDesc.ByteWidth = sizeof(unsigned long) * 6;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = t_indexBuffer;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	gfx.Device()->CreateBuffer(&indexBufferDesc, &indexData, &m_pIndexBuffer);
}

void CSprite::Init(CSurface surface, CD3DGraphics& gfx)
{
	if (m_bInitFromSurface)
		MEMORY::SafeDelete(m_pTexture);

	m_pTexture = new CTexture();
	m_pTexture->InitTexture(surface, gfx);

	m_bInitFromSurface = true;

	//CreateVertexBuffer(gfx);
}

void CSprite::CreateVertexBuffer(CD3DGraphics& gfx)
{
	MEMORY::DX::SafeRelease(m_pVertexBuffer);

	D3D11_BUFFER_DESC desc;
	D3D11_SUBRESOURCE_DATA data;
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.ByteWidth = sizeof(CUSTOMVERTEX) * NVERTS_SPRITE;
	desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;
	desc.Usage = D3D11_USAGE_DYNAMIC;

	CUSTOMVERTEX* temp = new CUSTOMVERTEX[NVERTS_SPRITE];

	data.pSysMem = temp;
	data.SysMemPitch = 0;
	data.SysMemSlicePitch = 0;

	if (FAILED(gfx.Device()->CreateBuffer(&desc, &data, &m_pVertexBuffer)))
		return;
}

D3DXMATRIX CSprite::GetWorldMatrix(CD3DGraphics& gfx)
{
	float x = 0, y = 0;

	y = -((-(int)(gfx.GetClientHeight()) / 2.0f) + m_yPos) / (gfx.GetClientHeight() / 2.0f);
	x = ((-(int)(gfx.GetClientWidth()) / 2.0f) + m_xPos) / (gfx.GetClientWidth() / 2.0f);

	D3DXMATRIX w;

	D3DXMATRIX r;
	D3DXMATRIX t1;
	D3DXMATRIX t2;
	D3DXMATRIX s;

	D3DXMatrixIdentity(&w);
	D3DXMatrixRotationZ(&r, m_rotation);
	D3DXMatrixTranslation(&t2, x, y, 0.0f);
	D3DXMatrixScaling(&s, m_xScale, m_yScale, 1.0f);
	w = s*r*t2;

	return w;
}

void CSprite::Rotate2DPoint(D3DXVECTOR2 & v, float x, float y, float ax, float ay)
{
	float s = sinf(m_rotation);
	float c = cosf(m_rotation);

	v.x = x - ax;
	v.y = y - ay;

	float xnew = v.x * c - v.y * s;
	float ynew = v.x * s + v.y * c;

	v.x = xnew + ax;
	v.y = ynew + ay;
}

void CSprite::Render(CD3DGraphics & gfx)
{
	if (!m_pTexture)
	{
		//ADD OUTPUT CLASS
		return;
	}

	D3DXMATRIX world;
	D3DXMatrixIdentity(&world);

	UINT stride = sizeof(CUSTOMVERTEX);
	UINT offset = 0;

	//Vertex buffer shouldnt need to be updated unless the color is changed 
	//(unless i think if some other fancy way of changing colours)
	//
	//if (m_flag_dirty)
	//{
	//	m_tempBuffer = GeneratePoints(gfx);


	//	if (!m_tempBuffer)
	//	{
	//		//ADD OUTPUT CLASS
	//		return;
	//	}

	//	D3D11_MAPPED_SUBRESOURCE resource;

	//	HRESULT result;
	//	result = gfx.Context()->Map(m_pVertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	//	if (FAILED(result))
	//		return;

	//	CUSTOMVERTEX* pData = (CUSTOMVERTEX*)resource.pData;
	//	memcpy(pData, m_tempBuffer, sizeof(CUSTOMVERTEX)*NVERTS_SPRITE);

	//	gfx.Context()->Unmap(m_pVertexBuffer, 0);
	//	MEMORY::SafeDeleteArr(m_tempBuffer);
	//}

	gfx.Context()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	gfx.Context()->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	gfx.Context()->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	D3DXMATRIX w = GetWorldMatrix(gfx);

	gfx.SetActiveShader(0);
	gfx.RenderShader(6,w,1,m_pTexture->GetTexture());
}

CUSTOMVERTEX* CSprite::GeneratePoints(CD3DGraphics& gfx)
{
	if (!m_pTexture)
		return NULL;

	float top, bottom, left, right;

	int width = m_pTexture->GetWidth() * m_xScale;
	int height = m_pTexture->GetHeight() * m_yScale;

	int x = m_xPos;
	int y = m_yPos;

	int spriteWidth = width;
	int spriteHeight = height;

	float a_x = m_anchorX;
	float a_y = m_anchorY;
	
	if (m_anchorX < 1.0f && m_anchorY < 1.0f)
	{
		a_x = m_anchorX*width;
		a_y = m_anchorY*height;
	}

	if (m_bFlip)
	{
		a_x = width - a_x;
	}


	if (m_areaLock.bottom != -1 && m_areaLock.left != -1 &&
		m_areaLock.right != -1 && m_areaLock.top != -1)
	{
		if (x > m_areaLock.right || x + width < m_areaLock.left ||
			y > m_areaLock.bottom || y + height < m_areaLock.top)
			return NULL;

		if (x - a_x < m_areaLock.left)
			x = m_areaLock.left;
		if (width + x - a_x > m_areaLock.right)
			spriteWidth = m_areaLock.right - m_xPos;
		if (height + y - a_y > m_areaLock.bottom)
			spriteHeight = m_areaLock.bottom - m_yPos;
		if (y - a_y < m_areaLock.top)
			y = m_areaLock.top;
	}

	D3DXVECTOR2 tl;
	D3DXVECTOR2 tr;
	D3DXVECTOR2 bl;
	D3DXVECTOR2 br;

	Rotate2DPoint(tl, x - a_x, y - a_y, m_xPos, m_yPos);
	Rotate2DPoint(tr, m_xPos + spriteWidth - a_x, y - a_y, m_xPos, m_yPos);
	Rotate2DPoint(bl, x - a_x, m_yPos + spriteHeight - a_y, m_xPos, m_yPos);
	Rotate2DPoint(br, m_xPos + spriteWidth - a_x, m_yPos + spriteHeight - a_y, m_xPos, m_yPos);

	int cheight = 0; //SYSTEM::GetWindowHeight();
	int cwidth = 0;// SYSTEM::GetWindowWidth();

	float tl_x = ((-cwidth / 2.0f) + tl.x) / (cwidth / 2.0f);
	float tl_y = -((-cheight / 2.0f) + tl.y) / (cheight / 2.0f);

	float tr_x = ((-cwidth / 2.0f) + tr.x) / (cwidth / 2.0f);
	float tr_y = -((-cheight / 2.0f) + tr.y) / (cheight / 2.0f);

	float bl_x = ((-cwidth / 2.0f) + bl.x) / (cwidth / 2.0f);
	float bl_y = -((-cheight / 2.0f) + bl.y) / (cheight / 2.0f);

	float br_x = ((-cwidth / 2.0f) + br.x) / (cwidth / 2.0f);
	float br_y = -((-cheight / 2.0f) + br.y) / (cheight / 2.0f);

	float leftUV = (float)(x - m_xPos) / (float)width;
	float rightUV = 1.0f-((float)(width - spriteWidth)) / (float)width;
	float topUV = (float)(y - m_yPos) / (float)height;
	float bottomUV = 1.0f-((float)(height - spriteHeight)) / (float)height;

	if (m_bFlip)
	{
		leftUV = 1.0f - leftUV;
		rightUV = 1.0f - rightUV;
	}

	D3DXVECTOR2 UV[4] = {
		{ leftUV ,topUV},
		{ rightUV,topUV},
		{ rightUV,bottomUV},
		{ leftUV ,bottomUV}
	};

	CUSTOMVERTEX* verts = new CUSTOMVERTEX[4];

	verts[0].position = D3DXVECTOR3(tl_x, tl_y, 0.5f);
	verts[1].position = D3DXVECTOR3(tr_x, tr_y, 0.5f);
	verts[2].position = D3DXVECTOR3(br_x, br_y, 0.5f);
	verts[3].position = D3DXVECTOR3(bl_x, bl_y, 0.5f);

	verts[0].UV = UV[0];
	verts[1].UV = UV[1];
	verts[2].UV = UV[2];
	verts[3].UV = UV[3];

	verts[0].color = verts[1].color = verts[2].color = verts[3].color = m_color;

	return verts;
}

int CSprite::GetWidth()
{
	if (!m_pTexture)
		return -1;
	return m_pTexture->GetWidth();
}

int CSprite::GetHeight()
{
	if (!m_pTexture)
		return -1;
	return m_pTexture->GetHeight();
}

int CSprite::GetAnchorX()
{
	int a = m_anchorX;
	if (m_anchorX < 1.0f)
		a = m_anchorX * GetWidth();
	return a;
}

int CSprite::GetAnchorY()
{
	int a = m_anchorY;
	if (m_anchorY < 1.0f)
		a = m_anchorY * GetHeight();
	return a;
}

int CSprite::GetX()
{
	return m_xPos;
}

int CSprite::GetY()
{
	return m_yPos;
}

float CSprite::GetScaleY()
{
	return m_yScale;
}

float CSprite::GetScaleX()
{
	return m_xScale;
}

float CSprite::GetRotation()
{
	return m_rotation;
}

void CSprite::SetRectLock(RECT r)
{
	m_areaLock = r;
	m_flag_dirty = true;
}

bool CSprite::WithinLock()
{
	int width = GetWidth() * GetScaleX();
	int height = GetHeight() * GetScaleY();

	float a_x = m_anchorX;
	float a_y = m_anchorY;

	if (a_x < 1.0f && a_y < 1.0f)
	{
		a_x *= width;
		a_y *= height;
	}

	if (m_areaLock.bottom != -1 && m_areaLock.left != -1 &&
		m_areaLock.right != -1 && m_areaLock.top != -1)
	{
		if (m_xPos + width - a_x < m_areaLock.left || m_xPos - a_x > m_areaLock.right ||
			m_yPos - a_y > m_areaLock.bottom || m_yPos + height - a_y < m_areaLock.top)
			return false;
	}

	if (m_xPos - a_x > 0/*SYSTEM::GetWindowWidth()*/ || m_yPos - a_x > 0/*SYSTEM::GetWindowHeight()*/ ||
		m_xPos + width - a_x < 0 || m_yPos + height - a_x < 0)
		return false;

	return true;
}

void CSprite::SetPosition(int x, int y)
{
	m_xPos = x;
	m_yPos = y;
	m_flag_dirty = true;
}

void CSprite::SetScaleX(float x)
{
	m_xScale = x;
	m_flag_dirty = true;
}

void CSprite::SetScaleY(float y)
{
	m_yScale = y;
	m_flag_dirty = true;
}

void CSprite::SetScale(float x, float y)
{
	m_xScale = x;
	m_yScale = y;
	m_flag_dirty = true;
}

void CSprite::SetColor(float r, float g, float b, float a)
{
	m_color = D3DXVECTOR4(r, g, b, a);
	m_flag_dirty = true;
}

void CSprite::SetColor(D3DXVECTOR4 color)
{
	m_color = color;
	m_flag_dirty = true;
}

void CSprite::Flip(bool b)
{
	m_bFlip = b;
	m_flag_dirty = true;
}

void CSprite::SetAnchor(float x, float y)
{
	m_anchorX = x;
	m_anchorY = y;
	m_flag_dirty = true;
}

void CSprite::SetRotation(float r)
{
	m_rotation = r;
	m_flag_dirty = true;
}

void CSprite::Resize(int width, int height)
{
	if (!m_pTexture)
		return;

	m_xScale = (float)width / (float)m_pTexture->GetWidth();
	m_yScale = (float)height / (float)m_pTexture->GetHeight();
	m_flag_dirty = true;
}
