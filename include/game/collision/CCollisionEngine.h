#pragma once

#include "../../CLog.h"

class CCollisionEngine
{
private:
	CCollisionEngine();
	CCollisionEngine* _instance;

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

private:

};