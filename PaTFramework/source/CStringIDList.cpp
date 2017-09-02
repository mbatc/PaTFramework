#include "CStringIDList.h"
#include "CLog.h"

CStringIDList::CStringIDList()
{}

CStringIDList::~CStringIDList()
{
	for (int i = 0; i < m_id_list.size(); i++)
	{
		delete [] m_id_list[i].c_string;
		m_id_list[i].c_string = NULL;
	}
}

bool CStringIDList::add_pair(const char * c_string, unsigned int id)
{
	char* msg = NULL;
	if (id_exists(id) && cstring_exists(c_string))
	{
		msg = "Entry with the same ID and C-String already exists";
	} 
	else if (id_exists(id))
	{
		msg = "Entry with the same ID already exists";
	} 
	else if (cstring_exists(c_string))
	{
		msg = "Entry with the same C-String already exists";
	}

	if (msg)
	{
		Log(this, DEBUGLOG_LEVEL_WARNING, "Unable to add string-id (%s, %d): %s",c_string, id,msg);
		return false;
	}

	ID_CSTRING add(c_string, id);

	m_id_list.push_back(add);
	return false;
}

const char * CStringIDList::get_str(unsigned int id)
{
	for (int i = 0; i < m_id_list.size(); i++)
	{
		if (m_id_list[i].id == id)
			return m_id_list[i].c_string;
	}

	return nullptr;
}

const char * CStringIDList::get_str_at_index(int index)
{
	if (index < 0 || index >= m_id_list.size())
		return "(null)";

	return m_id_list[index].c_string;
}

unsigned int CStringIDList::get_id(const char * c_string)
{
	for (int i = 0; i < m_id_list.size(); i++)
	{
		if (!strcmp(m_id_list[i].c_string, c_string))
			return m_id_list[i].id;
	}

	return UINTMAX_MAX;
}

unsigned int CStringIDList::get_id(int index)
{
	if (index < 0 || index >= m_id_list.size())
		return UINTMAX_MAX;

	return m_id_list[index].id;
}

bool CStringIDList::id_exists(unsigned int id) const
{
	for (int i = 0; i < m_id_list.size(); i++)
	{
		if (id == m_id_list[i].id)
			return true;
	}

	return false;
}

bool CStringIDList::cstring_exists(const char * c_string) const
{
	for (int i = 0; i < m_id_list.size(); i++)
	{
		if (!strcmp(m_id_list[i].c_string, c_string))
			return true;
	}
	return false;
}
