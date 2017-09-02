#pragma once

#include <cstdio>

class CDirectoryFileList
{
public:
	CDirectoryFileList();
	~CDirectoryFileList();

	void SetDirectory(char* directory);
	void SetFileExtension(char* extension);
	void FindFiles();

	char* GetFilename(int index);
	char* GetSubFolder(int index);

	int		GetNumberOfSubFolders() { return m_nSubFolders; }
	int		GetNumberOfFiles() { return m_nFiles; }
private:
	char*	m_pCurrentDirectory;
	char*	m_pFileType;

	int		m_nFiles;
	char**	m_ppFilename;

	int		m_nSubFolders;
	char**	m_ppSubFolder;
};