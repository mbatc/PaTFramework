#pragma once

#include "../../CLog.h"

#include "CColliderBase.h"
#include "CColliderAABB.h"
#include "CColliderSphere.h"
#include "CColliderTriangle.h" 
#include "CColliderMesh.h"

#define CREATE_COLLIDER(type) CCollisionEngine::get_instance()->create_collider<type>()
#define CREATE_COLLIDER(type, group) CCollisionEngine::get_instance()->create_collider<type>(group)

#define DESTROY_COLLIDER(collider) CCollisionEngine::get_instance()->unregister_collider(collider)

class CCollisionEngine
{
private:
	CCollisionEngine() {
		m_group.resize(0);
		init_collidertype_identifier();
	}
	static CCollisionEngine* _instance;
	
	struct ColliderListItem {
		CColliderBase*	_collider;
		unsigned int	_id;
	};

	struct ColliderGroup {
		ColliderGroup()
		:
			_name(""),
			_id(0),
			_flag(false)
		{}

		std::vector<ColliderListItem> _list;

		std::string		_name;
		unsigned int	_id;
		bool			_flag;
	};
public:
	static CCollisionEngine* get_instance() {
		if (!_instance)
		{
			_instance = new CCollisionEngine;
			if (!_instance)
			{
				Log(nullptr, DEBUGLOG_LEVEL_ERROR, "Unable to create CCollisionEngine instance");
				return nullptr;
			}
			Log(nullptr, DEBUGLOG_LEVEL_INFO, "CCollisionEngine instance created (ptr=%p)",_instance);
		}

		return _instance;
	}

	static void destroy_instance() {
		if (_instance)
		{
			_instance->cleanup();
			Log(nullptr, DEBUGLOG_LEVEL_INFO, "Destroying CCollisionEngine instance (ptr=%p)", _instance);
			delete _instance;
			_instance = nullptr;
		}
	}

	unsigned int update();

	template <class T> static bool is_type(CColliderBase* b) {
		return (get_instance()->get_typeidentifier<T>() == b->collider_id());
	}

	unsigned int get_typeidentifier(CColliderBase* b) {
		return b->collider_id();
	}
	template <class T> unsigned int get_typeidentifier() {
		T* temp = new T(); int id = temp->collider_id(); delete temp;
		return id;
	}
	
	template <class T> CColliderBase* create_collider(std::string group = "(null)") {
		int i = register_collider(new T,group);
		return get_group(group)._list[i]._collider;
	}

	CColliderBase* create_collider(int identifier, std::string group = "(null)") {
		int i = -1;
		if (identifier == get_typeidentifier<CColliderAABB>())
		{
			i = register_collider(new CColliderAABB(), group);
		}
		else if (identifier == get_typeidentifier<CColliderSphere>())
		{
			i = register_collider(new CColliderSphere(), group);
		}
		else if (identifier == get_typeidentifier<CColliderTriangle>())
		{
			i = register_collider(new CColliderTriangle(), group);
		}
		else if (identifier == get_typeidentifier<CColliderMesh>())
		{
			i = register_collider(new CColliderMesh(), group);
		}
		if (i < 0)
			return nullptr;

		return get_group(group)._list[i]._collider;
	}

	void destroy_collider(CColliderBase* col);
	void destroy_collider(int id);
private:
	void destroy_collider(CColliderBase* col, ColliderGroup group);
	void destroy_collider(int index, ColliderGroup group);

	unsigned int do_collision(CColliderBase* col, CColliderBase* col2);
	unsigned int do_collision_mesh(CColliderMesh* col, CColliderBase* col2, std::vector<int> face_index = std::vector<int>(0));
	unsigned int do_collision_mesh(CColliderMesh* col, CColliderMesh* col2, std::vector<int> face_index = std::vector<int>(0), std::vector<int> face_index2 = std::vector<int>(0));

	bool			check_collision(CColliderBase* col, CColliderBase* col2);
	void			add_collision_data(CColliderBase* col, CColliderBase* col2);

	unsigned int	do_mesh_collision(CColliderMesh* mesh, CColliderBase* col);
	unsigned int	do_mesh_collision(CColliderMesh* mesh, CColliderMesh* mesh2);

	unsigned int register_collider(CColliderBase* collider, std::string name);
	unsigned int unregister_collider(int id, ColliderGroup g);

	unsigned int register_group(std::string name);
	unsigned int unregister_group(int id);
	ColliderGroup& get_group(std::string name);

	unsigned int get_uniqueid();

	std::vector<ColliderGroup> m_group;

	void init_collidertype_identifier();
	template <class T> void set_colliderid(int id)	{
		CColliderBase* base = new T;
		base->collider_id() = id;
		delete base;
	}

	void cleanup();
};