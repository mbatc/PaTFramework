#include "CDirectoryFileList.h"
#include "helpers\Helper.h"

#include <windows.h>
#include <tchar.h> 
#include <stdio.h>
#include <strsafe.h>

CDirectoryFileList::CDirectoryFileList()
	:
	m_pCurrentDirectory(NULL),
	m_ppFilename(NULL)
{
	m_nFiles = 0;
	m_pFileType = NULL;
}

CDirectoryFileList::~CDirectoryFileList()
{
	MEMORY::SafeDeleteArr(m_pCurrentDirectory);
	for (int i = 0; i < m_nFiles; i++)
	{
		MEMORY::SafeDeleteArr(m_ppFilename[i]);
	}

	MEMORY::SafeDeleteArr(m_ppFilename);
}

void CDirectoryFileList::SetDirectory(char* directory)
{
	STRING::Copy(m_pCurrentDirectory, directory);
}

void CDirectoryFileList::SetFileExtension(char* extension)
{
	STRING::Copy(m_pFileType, extension);
}

void CDirectoryFileList::FindFiles()
{
	WIN32_FIND_DATA ffd;
	HANDLE fileHandle = INVALID_HANDLE_VALUE;
	DWORD dwError;

	bool checkFileType = (bool)(m_pFileType);

	if (!m_pCurrentDirectory)
		return;

	std::cout << "CDirectoryFileList: Finding Files in directory '" << m_pCurrentDirectory << "'...\n";

	fileHandle = FindFirstFile(m_pCurrentDirectory, &ffd); 
	if (fileHandle == INVALID_HANDLE_VALUE)
	{
		std::cout << "No Files In Directory...\n";
	}

	if (FindNextFile(fileHandle, &ffd) != 0)
		if (FindNextFile(fileHandle, &ffd) == 0)
			return;
	do
	{
		if (ffd.cFileName)
		{
			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_HIDDEN)
				continue;

			if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				char* temp = NULL;
				STRING::Copy(temp, ffd.cFileName);
				cout << ffd.cFileName << endl;
				MEMORY::LIST::AddToListArr(m_ppSubFolder, temp, m_nSubFolders);
			}
			else
			{
				char* temp = NULL;
				STRING::Copy(temp, ffd.cFileName);

				if (m_pFileType)
				{
					char* ext = FILEH::GetFileExtension(ffd.cFileName);

					if (!ext || strcmp(ext, m_pFileType))
						continue;
				}

				cout << ffd.cFileName << endl;
				MEMORY::LIST::AddToListArr(m_ppFilename, temp, m_nFiles);
			}
		}
	} while (FindNextFile(fileHandle, &ffd) != 0);

	dwError = GetLastError();

	if (dwError != ERROR_NO_MORE_FILES)
	{
		cout << "CDirectoryFileList: Something Went Wrong...\n";
	}

	FindClose(fileHandle);

	if (m_nFiles > 0)
	{
		cout << m_nFiles << " Files found in directory: " << m_pCurrentDirectory << endl;
		cout << m_nSubFolders << " Sub Folders found in directory: " << m_pCurrentDirectory << endl;
	}
}

char* CDirectoryFileList::GetFilename(int index)
{
	if (index >= m_nFiles || index < 0)
		return NULL;

	if (m_ppFilename)
	{
		if (m_ppFilename[index])
		{
			return m_ppFilename[index];
		}
	}

	return NULL;
}

char * CDirectoryFileList::GetSubFolder(int index)
{
	if (index >= m_nSubFolders || index < 0)
		return NULL;

	if (m_ppSubFolder)
	{
		if (m_ppSubFolder[index])
		{
			return m_ppSubFolder[index];
		}
	}

	return NULL;
}
