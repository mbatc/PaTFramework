#pragma once

#include "../../CLog.h"

#include "CColliderBase.h"

#define CREATE_COLLIDER(type) CCollisionEngine::get_instance()->create_collider<type>(collider)
#define DESTROY_COLLIDER(collider) CCollisionEngine::get_instance()->unregister_collider(collider)

class CCollisionEngine
{
private:
	CCollisionEngine();
	CCollisionEngine* _instance;

	struct ColliderListItem {
		CColliderBase*	_collider;
		unsigned int	_id;
	};
public:
	CCollisionEngine* get_instance() {
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
	}

	void destroy_instance() {
		if (_instance)
		{
			Log(nullptr, DEBUGLOG_LEVEL_INFO, "Destroying CCollisionEngine instance (ptr=%p)", _instance);
			delete _instance;
			_instance = nullptr;
		}
	}

	unsigned int update();

	template <class T> static bool is_type(CColliderBase* b) {
		return (T::collider_id() == b->collider_id());
	}
	
	template <class T> CColliderBase* create_collider() {
		int i = register_collider(new T);
		return m_collider[i];
	}

	void destroy_collider(CColliderBase* col);
	void destroy_collider(int id);
private:
	unsigned int check_collision(CColliderBase* col, CColliderBase* col2);

	unsigned int register_collider(CColliderBase* collider);
	unsigned int unregister_collider(int id);

	unsigned int get_uniqueid();

	std::vector<ColliderListItem> m_collider;
};