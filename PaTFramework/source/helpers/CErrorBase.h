#pragma once

#include "../CLog.h"

#include "../CAppErrDef.h"
#include "../CStringIDList.h"

#include "macro.h"

#define ERR(x) CError::get_instance()->err(x)
#define RET_ERR(x) return CError::get_instance()->err(x)
#define ADD_ERR(x) CError::get_instance()->add_err(x, TO_STR(x))
#define GETLASTERROR() CError::get_instance()->get_lasterror()

class CError
{
private:
	CError() {}

	static CError* _instance;
public:
	static CError* get_instance() {
		if (!_instance) {
			_instance = new CError();
			if (!_instance) {
				Log(nullptr, DEBUGLOG_LEVEL_ERROR, "CError instance could not be created");
				return nullptr;
			}

			_instance->load_defaulterrors();
			Log(nullptr, DEBUGLOG_LEVEL_INFO, "CError instance created (ptr=%p)", _instance);
		}
		
		return _instance;
	}

	static CError* destroy_instance() {
		if (_instance)
			delete _instance;

		_instance = nullptr;

		Log(nullptr, DEBUGLOG_LEVEL_INFO, "CError instance destroyed");
	}

	unsigned int get_lasterror() { return _last_error_; }
	unsigned int err(unsigned int e) { if (e != 0) log_err(e); return e; }

	bool add_err(unsigned int val, const char* name) {
		return m_errList.add_pair(name, val);
	}
	const char*		get_errorChar(unsigned int code) {
		return m_errList.get_str(code);
	}
private:
	//Internal functions for error logging
	void			log_err(unsigned int e) { if (e != PAT_NO_ERROR) { Log(this, DEBUGLOG_LEVEL_ERROR, get_errorChar(e)); } }
	void			load_defaulterrors();

	CStringIDList	m_errList;
	unsigned int _last_error_;
};