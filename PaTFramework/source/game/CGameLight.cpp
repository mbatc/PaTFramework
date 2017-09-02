#include "CGameLight.h"

CGameLight::CGameLight()
{
}

CGameLight::~CGameLight()
{
}

unsigned int CGameLight::startup()
{
	CGameObject::startup();
	return 0;
}

unsigned int CGameLight::update()
{
	CGameObject::update();
	return 0;
}

unsigned int CGameLight::render(CD3DGraphics& gfx)
{
	CGameObject::render(gfx);
	return 0;
}
