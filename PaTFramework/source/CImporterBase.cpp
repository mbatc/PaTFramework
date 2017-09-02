#include "CImporterBase.h"
#include "CIO.h"
#include "CLog.h"

#include "helpers\FileHelper.h"
#include "helpers\StringHelper.h"
#include "helpers/memoryhelper.h"

using namespace std;

CImporterBase::CImporterBase(string file)
	:
	_last_error(0)
{
	m_file = file;
}

CImporterBase::~CImporterBase()
{}

std::string CImporterBase::read_string()
{
	int len = read_int();
	char* buffer = 0;

	if (len <= 0)
		return "";
		
	buffer = new char[len + 1];
	_io.ReadF(m_file, buffer, sizeof(char), len);
	buffer[len] = '\0';

	return buffer;
}

int CImporterBase::read_int()
{
	int i = 0;
	_io.ReadF(m_file, &i, sizeof(int), 1);

	return i;
}

unsigned int CImporterBase::reimport()
{
	clean();
	_last_error = do_import();
	return getlasterror();
}

unsigned int CImporterBase::import(std::string file)
{
	clean();
	m_file = file;

	get_directory();

	_last_error = do_import();
	return getlasterror();
}

void CImporterBase::get_directory()
{
	char* temp = NULL;
	STRING::Copy(temp, m_file.c_str());
	int len = m_file.length();
	int dir_i;
	for (dir_i = len; dir_i >= 0 && temp[dir_i] != '/' && temp[dir_i] != '\\'; dir_i--);
	temp[dir_i + 1] = '\0';

	char* baseDirectory = 0;
	STRING::Copy(baseDirectory, temp);

	m_directory = baseDirectory;

	MEMORY::SafeDeleteArr(baseDirectory);
	MEMORY::SafeDeleteArr(temp);
}
