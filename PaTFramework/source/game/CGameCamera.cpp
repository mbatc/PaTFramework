#include "CGameCamera.h"

#define _USE_MATH_DEFINES
#include <math.h>
CGameCamera::CGameCamera()
{
}

CGameCamera::~CGameCamera()
{
}

unsigned int CGameCamera::startup()
{
	CGameObject::startup();
	reset_vectors();
	return 0;
}

unsigned int CGameCamera::update()
{
	CGameObject::update();
	return 0;
}

unsigned int CGameCamera::render(CD3DGraphics& gfx)
{
	reset_vectors();
	D3DXMATRIX view;

	D3DXMATRIX transform_rot;
	D3DXMatrixRotationYawPitchRoll(&transform_rot, get_transform().get_rotation().y, 
		get_transform().get_rotation().x,
		get_transform().get_rotation().z);

	D3DXVec3TransformNormal(&m_lookat, &m_lookat, &transform_rot);
	D3DXVec3TransformNormal(&m_up, &m_up, &transform_rot);

	m_lookat = m_eye + m_lookat;

	D3DXMatrixLookAtLH(&view, &m_eye, &m_lookat, &m_up);
	gfx.SetViewMatrix(view);

	CGameObject::render(gfx);
	return 0;
}

void CGameCamera::reset_vectors()
{
	m_lookat =	D3DXVECTOR3(0.0f,0.0f,1.0f);

	m_eye =		D3DXVECTOR3(get_transform().get_translation().x,
		get_transform().get_translation().y,
		get_transform().get_translation().z);

	m_up =		D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}
