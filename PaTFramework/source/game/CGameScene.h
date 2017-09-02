#pragma once

#include <vector>

#include "CGameObject.h"
#include "CGameLight.h"
#include "CGameCamera.h"

#include "../CResourceList.h"

class CGameScene
{
public:
	CGameScene();
	~CGameScene();

	unsigned int startup();
	unsigned int update();
	unsigned int render(CD3DGraphics& gfx);

	unsigned int get_lasterror() { return _last_error; }

	unsigned int add_object(CGameObject* obj = new CGameObject);
	unsigned int add_light(CGameLight* obj = new CGameLight);
	unsigned int add_camera(CGameCamera* obj = new CGameCamera);

	void rem_object(int index);
	void rem_light(int index);
	void rem_camera(int index);

	void rem_object(std::string name);
	void rem_light( std::string name);
	void rem_camera(std::string name);

	void* get_game_element(unsigned int ID);

	CResourceList<CGameObject>& get_objlist() { return m_object; }
	CResourceList<CGameLight>&	get_lgtlist() { return m_light; }
	CResourceList<CGameCamera>& get_camlist() { return m_camera; }
private:
	unsigned int get_unique_id();
	bool does_id_exist(unsigned int id);

	template <class T> void rem(std::vector<T> l, int index) {
		if (index < 0 || index >= l.size())
			return;

		l.erase(l.begin() + index);
	}
	unsigned int m_active_camera;

	CResourceList<CGameObject>	m_object;
	CResourceList<CGameLight>	m_light;
	CResourceList<CGameCamera>	m_camera;

	void do_updateanderr(CGameObject* obj);
	void do_renderanderr(CGameObject* obj, CD3DGraphics& gfx);
	void do_startupanderr(CGameObject* obj);
	void err(CGameObject* src, unsigned int e);

	unsigned int _last_error;
};