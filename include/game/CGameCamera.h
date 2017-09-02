#pragma once

#include "CGameObject.h"

class CGameCamera : public CGameObject
{
public:
	CGameCamera();
	~CGameCamera();

	unsigned int startup();

	unsigned int update();
	unsigned int render(CD3DGraphics& gfx);
private:
	void reset_vectors();

	D3DXVECTOR3 m_up;
	D3DXVECTOR3 m_eye;
	D3DXVECTOR3 m_lookat;
};