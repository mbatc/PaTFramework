#pragma once

#include <vector>
#include <string>

class CStringIDList
{
public:
	struct ID_CSTRING
	{
		ID_CSTRING(const char* c_string, unsigned int id)
			:
			id(id)
		{
			this->c_string = new char[strlen(c_string)+1];
			strcpy(this->c_string, c_string);
		}

		char* c_string;
		unsigned int id;
	};

	CStringIDList();
	~CStringIDList();
	bool add_pair(const char* c_string, unsigned int id);
	
	const char* get_str(unsigned int id);
	const char* get_str_at_index(int index);
	unsigned int get_id(const char* c_string);
	unsigned int get_id(int index);

	int count() { return m_id_list.size(); }
private:
	std::vector<ID_CSTRING> m_id_list;

	bool id_exists(unsigned int id) const;
	bool cstring_exists(const char* c_string) const;
};
