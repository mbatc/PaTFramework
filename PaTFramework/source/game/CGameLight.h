#pragma once

#include "CGameObject.h"

class CGameLight : public CGameObject
{
public:
	CGameLight();
	~CGameLight();

	unsigned int startup();

	unsigned int update();
	unsigned int render(CD3DGraphics& gfx);
private:
};