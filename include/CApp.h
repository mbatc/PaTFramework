#pragma once
#include "CAppErrDef.h"
//------------------------------------------


#include "CWindow.h"
#include "CTimer.h"
#include "game\CGame.h"

#include "input\CKeyboardInput.h"
#include "input\CMouseInput.h"
#include "input\CTouchInput.h"
#include "CStringIDList.h"
#include "CD3DGraphics.h"

#include "CLog.h"
#include "helpers\CErrorBase.h"

#include <string>

struct APPVERSION
{
	APPVERSION(int maj = 0, int min = 0, int patch = 0)
		:
		major(maj),
		minor(min),
		patch(patch)
	{}
	int major;
	int minor;
	int patch;
};

//The window procedure has been nested in this class so that it can access the non-public members of the CApp class
class CApp;
class CAppBridge;
class CAppHookBase;

class CAppWndProc
{
	friend CApp;

	CAppWndProc()
	{}

	static CAppWndProc* _instance;
	static CApp* g_toothbrush;
public:
	static LRESULT CALLBACK AppWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

	static CAppWndProc* get_instance()
	{
		if (!_instance)
		{
			_instance = new CAppWndProc();
			if(!_instance)
				Log(NULL, DEBUGLOG_LEVEL_FATAL, "Failed to create CAppWndProc instance");
		}
		return _instance;
	}
};

class CApp
{
	friend CAppWndProc;
	friend CAppBridge;
private:
	//-------------------------------------------------
	//Singleton design defs
	CApp(APPVERSION ver = APPVERSION());
	static CApp* _instance;

public:
	~CApp();
	static CApp* get_instance()
	{
		if (!_instance)
		{
			_instance = new CApp(APPVERSION(0, 1, 3));
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CApp instance created (ptr=%p)", _instance);
		}
		return _instance;
	}
	static void destroy_instance()
	{
		if (_instance)
			delete _instance;
		_instance = 0;
	}
	//--------------------------------------------------


	//Must be derived from CAppHookBase
	template <class T>unsigned int set_apphooktype();

	static void set_app_version(APPVERSION v) { _app_build_ = v; }
	static void set_app_name(std::string name = "PaTFramework Application") { _app_name_ = name; }

	unsigned int startup(int argc, char** argv, HINSTANCE hInstance);
	unsigned int run();
	unsigned int shutdown();

	//Sets the exit flag to true (starts app shutdown)
	void set_exitflag() { Log(this, DEBUGLOG_LEVEL_INFO, "exit_flag set (_last_error_=%s)", 
		CError::get_instance()->get_errorChar(GETLASTERROR())); _exit_flag = true; };

	void set_dTimeMultiplier(float mult) { m_dTimeMult = mult; }

private:
	void do_startup_log(int argc, char** argv);
	unsigned int init_console();
	unsigned int init_app_bridge();
	unsigned int init_window(HINSTANCE hInstance);
	unsigned int init_gfx();
	unsigned int init_input();
	unsigned int init_resources();
	unsigned int init_settings();
	unsigned int init_game();

	void init_log();

	unsigned int update_game();
	unsigned int update_system();

	//Display
	CWindow m_main_window;
	CD3DGraphics m_sysGfx;

	//Audio

	//Input
	CKeyboardInput	m_sysClient_keyboard;
	CMouseInput		m_sysClient_mouse;
	CTouchInput		m_sysClient_touch;

	CKeyboardInputState m_sysState_keyboard;
	CMouseInputState	m_sysState_mouse;
	CTouchInputState	m_sysState_touch;

	//Game
	CGame*				m_sysPtr_game;
	CTimer				m_sysFrameTimer;
	float				m_lastFrameTime;
	float				m_dTimeMult;
	//Settings

	//Resources

	//Versioning
	static APPVERSION	_app_build_;
	static std::string	_app_build_str;
	static std::string	_app_name_;

	APPVERSION		_PaTframework_build_;
	std::string		_PaTframework_build_str;

	bool _exit_flag;
	CAppHookBase* m_app_hook;
};

class CAppBridge
{
private:
	CAppBridge(CApp* app) {
		m_app = app;
	}

	static CAppBridge* _instance;
public:
	static CAppBridge* get_instance() {
		if (!_instance)
		{
			_instance = new CAppBridge(CApp::get_instance());
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CAppBridge instance created (ptr=%p)", _instance);
		}
		return _instance;
	}

	static void destroy_instance() {
		if (_instance)
		{
			delete _instance;
			Log(NULL, DEBUGLOG_LEVEL_INFO, "CAppBridge destroyed (ptr=%p)", _instance);
		}
		_instance = 0;
	}

	CD3DGraphics*	get_sysGfx() {
		return &m_app->m_sysGfx;
	}

	CKeyboardInput* get_sysKeyboard() {
		return &m_app->m_sysClient_keyboard;
	}

	CMouseInput*	get_sysMouse() {
		return &m_app->m_sysClient_mouse;
	}

	CTouchInput*	get_sysTouch() {
		return &m_app->m_sysClient_touch;
	}

	CGame*			get_game() {
		return m_app->m_sysPtr_game;
	}

	CWindow&		get_main_window() {
		return m_app->m_main_window;
	}

private:
	CApp* m_app;
};

class CAppHookBase
{
	friend CApp;
public:
	CAppHookBase() {}
	~CAppHookBase() {}

protected:
	virtual void on_init_resource() {};
	virtual void on_init_settings() {};
	virtual void on_init_game() {};

	//Return true if the main windows parameters are set in this function
	//This will prevent the default values from being used
	virtual bool on_init_window() { return false; };

	//Should be used to initialise graphics related systems such as shaders
	virtual void on_init_gfx() {};

	virtual void on_update_game() {};
	virtual void on_update_system() {};

	virtual void on_render_game() {};
};

template<class T>
inline unsigned int CApp::set_apphooktype()
{
	if (m_app_hook)
		MEMORY::SafeDelete(m_app_hook);
	m_app_hook = new T;

	return 0;
}
