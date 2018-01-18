#pragma once

#include "../CLog.h"

#include "CGameScene.h"
#include "../CD3DGraphics.h"

#include <vector>

class CGame
{
private:
	CGame()
	{
		Log(this, DEBUGLOG_LEVEL_INFO, "CGame instance created (ptr=%p)", this);
	}
	static CGame* _instance;
public:
	static CGame* get_instance() {
		if (!_instance)
			_instance = new CGame;
		Log(NULL, DEBUGLOG_LEVEL_INFO, "CGame pointer retrieved (ptr=%p)", _instance);
		return _instance;
	}
	void startup();

	void do_update(float deltaTime);
	void do_render(CD3DGraphics& gfx);

	unsigned int get_lasterror();

	CGameScene* get_scene() {
		return m_pScene;
	};

	static float dTime;
private:
	CGameScene* m_pScene;
	unsigned int _last_error;
};