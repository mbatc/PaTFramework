#include "CGameScene.h"
#include "../CLog.h"

CGameScene::CGameScene()
{}

CGameScene::~CGameScene()
{}

unsigned int CGameScene::startup()
{
	for (int i = 0; i < m_object.get_count(); i++)
	{
		do_startupanderr(m_object.get_resource_at_index(i));
	}

	for (int i = 0; i < m_light.get_count(); i++)
	{
		do_startupanderr(m_light.get_resource_at_index(i));
	}

	for (int i = 0; i < m_camera.get_count(); i++)
	{
		do_startupanderr(m_camera.get_resource_at_index(i));
	}
	return get_lasterror();
}

unsigned int CGameScene::update()
{
	for (int i = 0; i < m_object.get_count(); i++)
	{
		do_updateanderr(m_object.get_resource_at_index(i));
	}

	for (int i = 0; i < m_light.get_count(); i++)
	{
		do_updateanderr(m_light.get_resource_at_index(i));
	}

	for (int i = 0; i < m_camera.get_count(); i++)
	{
		do_updateanderr(m_camera.get_resource_at_index(i));
	}
	return get_lasterror();
}

unsigned int CGameScene::render(CD3DGraphics& gfx)
{
	for (int i = 0; i < m_object.get_count(); i++)
	{
		do_renderanderr(m_object.get_resource_at_index(i), gfx);
	}

	for (int i = 0; i < m_light.get_count(); i++)
	{
		do_renderanderr(m_light.get_resource_at_index(i), gfx);
	}

	for (int i = 0; i < m_camera.get_count(); i++)
	{
		do_renderanderr(m_camera.get_resource_at_index(i), gfx);
	}

	return get_lasterror();
}

unsigned int CGameScene::add_object(CGameObject * obj)
{
	if (!obj)
		return 0;

	obj->_ID_ = get_unique_id();
	return m_object.add_resource(obj, false, obj->_ID_, obj->get_name());
}

unsigned int CGameScene::add_light(CGameLight * obj)
{
	if (!obj)
		return 0;

	obj->_ID_ = get_unique_id();
	return m_light.add_resource(obj, false, obj->_ID_, obj->get_name());
}

unsigned int CGameScene::add_camera(CGameCamera * obj)
{
	if (!obj)
		return 0;

	obj->_ID_ = get_unique_id();
	return m_camera.add_resource(obj, false, obj->_ID_, obj->get_name());
}

void CGameScene::rem_object(int index)
{
	m_object.rem_resource_at_index(index);
}

void CGameScene::rem_light(int index)
{
	m_light.rem_resource_at_index(index);
}

void CGameScene::rem_camera(int index)
{
	m_camera.rem_resource_at_index(index);
}

void CGameScene::rem_object(std::string name)
{
	m_object.rem_resource(name);
}

void CGameScene::rem_light(std::string name)
{
	m_light.rem_resource(name);
}

void CGameScene::rem_camera(std::string name)
{
	m_camera.rem_resource(name);
}

void* CGameScene::get_game_element(unsigned int ID)
{
	void* element = nullptr;
	element = m_object.get_resource(ID);
	if (element)
		return element;

	element = m_light.get_resource(ID);
	if (element)
		return element;

	element = m_camera.get_resource(ID);
	if (element)
		return element;

	return nullptr;
}

unsigned int CGameScene::get_unique_id()
{
	unsigned int id = rand() * MAXUINT;
	
	if (does_id_exist(id))
		return get_unique_id();
	
	return id;
}

bool CGameScene::does_id_exist(unsigned int id)
{
	if (get_game_element(id))
		return true;

	return false;
}

void CGameScene::do_updateanderr(CGameObject * obj)
{
	err(obj, obj->update());
}

void CGameScene::do_renderanderr(CGameObject * obj, CD3DGraphics& gfx)
{
	err(obj, obj->render(gfx));
}

void CGameScene::do_startupanderr(CGameObject * obj)
{
	err(obj, obj->startup());
}

void CGameScene::err(CGameObject * src, unsigned int e)
{
	_last_error = e;

	if (e != 0)
	{
		Log(src, DEBUGLOG_LEVEL_ERROR, "Gameobject return error (%d)", e);
	}
}
