#pragma once

#include <string>

class CImporterBase
{
public:
	CImporterBase(std::string file);
	~CImporterBase();

	std::string		read_string();
	int				read_int();

	//Reimport the file passed to the constructor (call after get_data() to continue using importer)
	unsigned int reimport();

	//Import the file passed to the function
	virtual unsigned int import(std::string file);

	unsigned int getlasterror() { return _last_error; }
protected:
	virtual void			clean() = 0;
	virtual unsigned int	do_import() = 0;

	virtual void			zero() = 0;

	void get_directory();

	std::string		m_directory;
	std::string		m_file;
	unsigned int	_last_error;
};