#pragma once

#include <vector>
#include <string>

#include "helpers\MemoryHelper.h"

template <class res_type> class CResourceList
{
private:
	struct SRES
	{
		void clean() {
			if (_allow_delete)
				MEMORY::SafeDelete(_resource);
			zero();
		}

		void zero() {
			_resource = 0;
			_name.clear();
			_id = 0;
			_allow_delete = false;
			_filename.clear();
		}

		res_type* _resource;
		std::string _name;
		unsigned int _id;

		bool _allow_delete;
		std::string _filename;
	};

public:
	CResourceList();
	~CResourceList();

	std::string get_resource_name(int id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._id == id)
				return m_data[i]._name;
		}
	}

	std::string get_resource_name_at_index(int index) {
		if (index < 0 || index >= m_data.size())
			return "[invalid index]";

		return m_data[index]._name;
	}
	std::string get_resource_name(res_type* res) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._resource == res)
				return m_data[i]._name;
		}
	}

	bool		does_resource_exist(res_type* res) {
		for (int i = 0; i < m_data.size(); i++) {
			if (!does_resource_exist_at_index(i))
				continue;
			if (m_data[i]._resource == res)
				return true;
		}

		return false;
	}

	bool		does_resource_exist(unsigned int* id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (!does_resource_exist_at_index(i))
				continue;
			if (m_data[i]._id == id)
				return true;
		}

		return false;
	}

	bool		does_resource_exist_at_index(int index) {
		if (index < 0 || index >= m_data.size())
			return false;
		if (m_data[index]._resource == nullptr)
			return false;

		return true;
	}

	unsigned int get_count() {
		return m_data.size();
	}

	void		set_resource_name(unsigned int id, std::string name) {
		for (int i = 0; i < m_data.size(); i++) {
			if (!does_resource_exist_at_index(i))
				continue;
			if (m_data[i]._id == id)
				m_data[i]._name = name;
		}
	}
	void		set_resource_name(res_type* data, std::string name) {
		for (int i = 0; i < m_data.size(); i++) {
			if (!does_resource_exist_at_index(i))
				continue;
			if (m_data[i]._resource == data)
				m_data[i]._name = name;
		}
	}
	void		set_resource_name_at_index(int index, std::string name) {
		if (does_resource_exist_at_index(index))
			m_data[i]._name = name;
	}

	int			add_resource(res_type* data, bool allow_delete, unsigned int id = 0, std::string name = "(null)", std::string filename = "(null)") {
		SRES _new;
		_new._resource = data;
		_new._name = name;
		_new._id = id;
		_new._allow_delete = allow_delete;
		_new._filename = filename;

		m_data.push_back(_new);

		return id;
	}

	res_type*	get_resource(std::string name) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._name == name)
				return m_data[i]._resource;
		}

		return nullptr;
	}
	res_type*	get_resource(unsigned int id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._id == id)
				return m_data[i]._resource;
		}

		return nullptr;
	}
	res_type*	get_resource_at_index(int index) {
		if (!does_resource_exist_at_index(index))
			return nullptr;

		return m_data[index]._resource;
	}

	void		rem_resource_at_index(int index) {
		if (index < 0 || index >= m_data.size())
			return;

		m_data[index].clean();
		m_data.erase(m_data.begin() + index);
	}
	void		rem_resource(unsigned int id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._id == id) {
				m_data[i].clean();
				m_data.erase(m_data.begin() + i);
			}
		}
	}
	void		rem_resource(res_type* data) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._resource == data) {
				m_data[i].clean();
				m_data.erase(m_data.begin() + i);
				return;
			}
		}
	}
	void		rem_resource(std::string name) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._name == name) {
				m_data[i].clean();
				m_data.erase(m_data.begin() + i);
				return;
			}
		}
	}

	bool		allowdelete(int i) {
		if (!does_resource_exist_at_index(i))
			return false;
		return m_data[i]._allow_delete;
	}
	bool		allowdelete(res_type* res) {
		for (int i = 0; i < m_data.size(); i++) {
			if (res != m_data[i]._resource)
				continue;
			if (!does_resource_exist_at_index(i))
				return false;
			return m_data[i]._allow_delete;
		}

		return false;
	}
	bool		allowdelete_id(unsigned int id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (id != m_data[i]._id)
				continue;
			if (!does_resource_exist_at_index(i))
				return false;
			return m_data[i]._allow_delete;
		}

		return false;
	}

	unsigned int get_resource_count_in_file(std::string filename) {
		unsigned int count = 0;
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._filename == filename)
				count++;
		}

		return count;
	}

	res_type*	get_resource_from_file_at_index(std::string filename, int res_index) {
		int file_index = 0;
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._filename == filename) {
				if (file_index == res_index)
					return m_data[i]._resource;

				file_index++;
			}
		}
		return nullptr;
	}
	res_type*	get_resource_from_file(std::string filename, std::string name) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._filename == filename) {
				if (m_data[i]._name == name)
					return m_data[i]._resource;
			}
		}
		return nullptr;
	}

	std::string get_resource_filename(int id) {
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._id == id)
				return m_data[i]._filename;
		}
		return "(null)";
	}

	std::string get_resource_filename_at_index(int index) {
		if (!does_resource_exist_at_index(index))
			return "(null)";

		return m_data[index]._filename;
	}
private:
	std::vector<SRES> m_data;
};


template<class res_type>
inline CResourceList<res_type>::CResourceList()
{}

template<class res_type>
inline CResourceList<res_type>::~CResourceList()
{
	{
		for (int i = 0; i < m_data.size(); i++) {
			if (m_data[i]._allow_delete)
				delete m_data[i]._resource;
		}
	}
}