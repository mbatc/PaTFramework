#include "CApp.h"

#include <iostream>
#include <stdio.h>
#include <io.h>
#include <fcntl.h>

#include "CImporter.h"

#include "CResourceBank.h"
#include "game\collision\CCollisionEngine.h"
#include "game\components\CComponentFactory.h"
#include "helpers\CErrorBase.h"

using namespace std;

CApp* CApp::_instance = 0;
CApp* CAppWndProc::g_toothbrush = 0;
CAppWndProc* CAppWndProc::_instance = 0;
CAppBridge*	CAppBridge::_instance = 0;

APPVERSION CApp::_app_build_ = APPVERSION();
std::string CApp::_app_build_str;
std::string CApp::_app_name_ = "PaTFramework Application";

CApp::CApp(APPVERSION ver)
	:
	_exit_flag(false)
{
	char temp[48];
	sprintf(temp, "v(%d.%d.%d)", ver.major, ver.minor, ver.patch);
	_PaTframework_build_ = ver;
	_PaTframework_build_str = temp;

	sprintf(temp, "v(%d.%d.%d)", _app_build_.major, _app_build_.minor, _app_build_.patch);
	_app_build_str = temp;
	
	init_log();
	//Set the build version string
	Log(this, DEBUGLOG_LEVEL_INFO, "CApp instance(ptr=%p, %s) created", this, _PaTframework_build_str.c_str());
}

CApp::~CApp()
{
	Log(this, DEBUGLOG_LEVEL_INFO, "CApp instance(ptr=%p) destroyed", this);
}

unsigned int CApp::startup(int argc, char** argv, HINSTANCE hInstance)
{
	do_startup_log(argc, argv);

	if (init_app_bridge()) set_exitflag();
	if (init_input()) set_exitflag();
	if (init_window(hInstance)) set_exitflag();
	if (init_gfx()) set_exitflag();
	if (init_settings()) set_exitflag();
	if (init_resources()) set_exitflag();
	if (init_game()) set_exitflag();

	return GETLASTERROR();
}

unsigned int CApp::run()
{
	Log(this, DEBUGLOG_LEVEL_INFO, "CApp::run() entered");
	MSG _win_msg;

	while (!_exit_flag)
	{
		if (PeekMessage(&_win_msg, NULL, NULL, NULL, PM_REMOVE))
		{
			TranslateMessage(&_win_msg);
			DispatchMessage(&_win_msg);
		}
		else
		{
			//Do game and system frame by frame updates
			update_game();
			update_system();
		}
	}

	return GETLASTERROR();
}

unsigned int CApp::shutdown()
{
	CResourceBank::get_instance()->shutdown();
	CResourceBank::destroy();
	CImporter::destroy_instance();
	CCollisionEngine::destroy_instance();
	CError::destroy_instance();
	CComponentFactory::destroy_instance();



	return GETLASTERROR();
}

void CApp::do_startup_log(int argc, char** argv)
{
	//Setup the application and output some information to the console for debugging purposes
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "PaTFramework v(%d.%d.%d)", _PaTframework_build_.major, _PaTframework_build_.minor, _PaTframework_build_.patch);
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "%s v(%d.%d.%d)", _app_name_.c_str() ,_app_build_.major, _app_build_.minor, _app_build_.patch);
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "argc: %d", argc);
	for (int i = 0; i < argc; i++)
	{
		_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "argv[%d]: %s", i, argv[i]);
	}
}

unsigned int CApp::init_console()
{
#ifdef _DEBUG
	AllocConsole();
	freopen("CONIN$", "r", stdin);
	freopen("CONOUT$", "w", stdout);
	freopen("CONOUT$", "w", stderr);
#endif
	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_app_bridge()
{
	//Set the window procedures application pointer
	CAppWndProc* proc = CAppWndProc::get_instance();
	if (!proc)
		RET_ERR(PAT_ERR_OUT_OF_MEMORY);

	proc->g_toothbrush = this;
	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_window(HINSTANCE hInstance)
{
	bool use_default = true;

	m_main_window.SetWindowClassName("ProjectAberrantToothbrush-wndcls");
	m_main_window.SetWindowName("Project Aberrant Toothbrush");
	m_main_window.SetWindowStyle(WS_OVERLAPPEDWINDOW);

	if (m_app_hook)
		if (m_app_hook->on_init_window())
			use_default = false;
	if (use_default)
	{	
		if (!m_main_window.Init(hInstance, 960, 480, CAppWndProc::AppWndProc))
			RET_ERR(PAT_ERR_REGISTER_WND_FAILED);
	}
	else
	{
		if (!m_main_window.Init(hInstance, m_main_window.GetWidth(), m_main_window.GetHeight(), CAppWndProc::AppWndProc))
			RET_ERR(PAT_ERR_REGISTER_WND_FAILED);
	}

	if(!m_main_window.CWndCreateWindow())
		RET_ERR(PAT_ERR_CREATE_WND_FAILED);
	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_gfx()
{
	if (m_sysGfx.InitDX(&m_main_window))
		RET_ERR(PAT_ERR_DXINIT_FAILED);
	m_sysGfx.SetClearColor(0.3f, 0.3f, 0.3f, 1.0f);
	m_sysGfx.SetCurrentTarget(&m_main_window);

	if (m_app_hook)
		m_app_hook->on_init_gfx();
	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_input()
{
	Log(this, DEBUGLOG_LEVEL_ALL, "system keyboard input state set");
	m_sysClient_keyboard.SetKeyboardState(&m_sysState_keyboard);
	Log(this, DEBUGLOG_LEVEL_ALL, "system mouse input state set");
	m_sysClient_mouse.SetMouseState(&m_sysState_mouse);
	Log(this, DEBUGLOG_LEVEL_ALL, "system touch input state set");
	m_sysClient_touch.SetState(&m_sysState_touch);

	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_resources()
{
	if (m_app_hook)
		m_app_hook->on_init_resource();

	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::init_settings()
{
	RET_ERR(PAT_NO_ERROR);

	if (m_app_hook)
		m_app_hook->on_init_settings();
}

unsigned int CApp::init_game()
{
	m_sysPtr_game = CGame::get_instance();
	if (!m_sysPtr_game)
	{
		Log(this, DEBUGLOG_LEVEL_FATAL, "Unable to retrieve CGame instance");
		RET_ERR(PAT_ERR_OUT_OF_MEMORY);
	}
	m_sysPtr_game->startup();

	if (m_app_hook)
		m_app_hook->on_init_game();

	RET_ERR(PAT_NO_ERROR);
}

void CApp::init_log()
{
	std::string logfile(std::string("Log\\log-") + _app_build_str + std::string(".txt"));

	_LOG.CMDLog(true);
	_LOG.SetLogLevel(DEBUGLOG_LEVEL_ALL);
	_LOG.SetLogFile(logfile);
	init_console();
	_LOG.Log(this, DEBUGLOG_LEVEL_INFO, "_LOG initialised");
}

unsigned int CApp::update_game()
{
	//Game update
	m_sysPtr_game->do_update();
	if (m_app_hook)
		m_app_hook->on_update_game();
	//Game render
	if (m_sysGfx.BeginFrame())
		RET_ERR(PAT_ERR_GFX_BEGINFAILED);
	m_sysPtr_game->do_render(m_sysGfx);
	if (m_sysGfx.EndFrame())
		RET_ERR(PAT_ERR_GFX_ENDFAILED);

	if (m_app_hook)
		m_app_hook->on_render_game();

	RET_ERR(PAT_NO_ERROR);
}

unsigned int CApp::update_system()
{
	m_sysState_keyboard.Update();
	m_sysState_mouse.Update();
	m_sysState_touch.Update();

	if (m_app_hook)
		m_app_hook->on_update_system();

	RET_ERR(PAT_NO_ERROR);
}

LRESULT CAppWndProc::AppWndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	if (!g_toothbrush)
		g_toothbrush = CApp::get_instance();

	switch (msg)
	{
	//////////////////////////////////////////////////////////////////////
	//WINDOWS KEYBOARD MESSAGES
	case WM_KEYDOWN:
		g_toothbrush->m_sysState_keyboard.OnKeyPressed(wParam);
		return true;
	case WM_KEYUP:
		g_toothbrush->m_sysState_keyboard.OnKeyReleased(wParam);
		return true;
	case WM_CHAR:
		g_toothbrush->m_sysState_keyboard.OnCharacterPressed(wParam);
		return true;


	//////////////////////////////////////////////////////////////////////
	//WINDOWS MOUSE MESSAGES
	case WM_LBUTTONDOWN:
		g_toothbrush->m_sysState_mouse.OnLeftMousePressed();
		return true;
	case WM_LBUTTONUP:
		g_toothbrush->m_sysState_mouse.OnLeftMouseRelesed();
		return true;
	case WM_RBUTTONDOWN:
		g_toothbrush->m_sysState_mouse.OnRightMousePressed();
		return true;
	case WM_RBUTTONUP:
		g_toothbrush->m_sysState_mouse.OnRightMouseReleased();
		return true;
	case WM_MOUSEHWHEEL:
		g_toothbrush->m_sysState_mouse.OnHorizontalScroll(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_MOUSEWHEEL:
		g_toothbrush->m_sysState_mouse.OnVerticalScroll(GET_WHEEL_DELTA_WPARAM(wParam));
		break;
	case WM_MOUSEMOVE:
		g_toothbrush->m_sysState_mouse.SetMouse(LOWORD(lParam), HIWORD(lParam));
		break;
	//////////////////////////////////////////////////////////////////////
	//WINDOWS TOUCH 
	case WM_TOUCH:
		g_toothbrush->m_sysState_touch.OnTouch(wParam, lParam);
		return 0;
	//////////////////////////////////////////////////////////////////////
	case WM_SIZE:
		g_toothbrush->m_sysGfx.ResizeClient(hWnd, (UINT)LOWORD(lParam), (UINT)HIWORD(lParam));
		break;
	case WM_CLOSE:
		PostQuitMessage(0);
		g_toothbrush->set_exitflag();
		return DefWindowProc(hWnd, msg, wParam, lParam);
	case WM_DESTROY:
		PostQuitMessage(0);
		g_toothbrush->set_exitflag();
		return DefWindowProc(hWnd, msg, wParam, lParam);
	default:
		return DefWindowProc(hWnd, msg, wParam, lParam);
	}
	return DefWindowProc(hWnd, msg, wParam, lParam);
}
