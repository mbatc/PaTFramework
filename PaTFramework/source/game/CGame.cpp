#include "CGame.h"
#include "../CResourceBank.h"

#include "components\CComponentFactory.h"

#define _USE_MATH_DEFINES
#include <math.h>

CGame* CGame::_instance = 0;

void CGame::startup()
{
	//Initialise the game scene
	CComponentFactory* cfactory = CComponentFactory::get_instance();
	MEMORY::SafeDelete(m_pScene);
	m_pScene = new CGameScene;

	//load_startup_scene();

	if (m_pScene)
	{
		if ((_last_error = m_pScene->startup()) != 0)
			Log(this, DEBUGLOG_LEVEL_ERROR, "CGame::m_pScene->startup() return with error (%d)", _last_error);
	}
	else
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "CGame::m_pScene=(null), unable to run startup()");
	}
}

void CGame::do_update()
{
	if (m_pScene)
	{
		if ((_last_error = m_pScene->update()) != 0)
			Log(this, DEBUGLOG_LEVEL_ERROR, "CGame::m_pScene->update() return with error (%d)", _last_error);
	}
	else
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "CGame::m_pScene=(null)");
	}
}

void CGame::do_render(CD3DGraphics& gfx)
{
	if (m_pScene)
	{
		if ((_last_error = m_pScene->render(gfx)) != 0)
			Log(this, DEBUGLOG_LEVEL_ERROR, "CGame::m_pScene->update() return with error (%d)", _last_error);
	}
	else
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "CGame::m_pScene=(null)");
	}
}

unsigned int CGame::get_lasterror()
{
	return _last_error;
}
