#include "FileDealer.h"

#include <iostream>
#include <fstream>
#include <direct.h>
#include <io.h>
#include "dirent_ls.h"

/*
* make the dir recursively
* if _dir = "a/b/c"
* than a, a/b, a/b/c will be made
*/
int FileDealer::CreatDir(std::string _dir)
{
	const char *pDir = _dir.c_str();
	int i = 0;
	int iRet;
	int iLen;
	char* pszDir;

	if (NULL == pDir)
		return 0;

	pszDir = _strdup(pDir);
	iLen = (int)strlen(pszDir);

	// �����м�Ŀ¼
	for (i = 0; i < iLen; i++)
	{
		if (pszDir[i] == '\\' || pszDir[i] == '/')
		{
			pszDir[i] = '\0';
			//���������,����
			iRet = _access(pszDir, 0);
			if (iRet != 0)
			{
				iRet = _mkdir(pszDir);
				if (iRet != 0)
					return -1;
			}
			//֧��linux,������\����/
			pszDir[i] = '/';
		}
	}
	iRet = _mkdir(pszDir);
	free(pszDir);
	return iRet;
}

/*
* Function : Get all the file names in the folder and is sub folders recursively
* Input : 
*	folder name
* Output : 
*	file names, output path is ralative to the input _dir
*	is success, return ture; otherwise, return false
*/
bool FileDealer::GetFileList(std::string _dir, std::vector<std::string>& _file_names)
{
	_file_names.clear();
	DIR *p;
	struct dirent *dirp;
	if ((p = opendir(_dir.c_str())) == NULL)
		return false;

	while ((dirp = readdir(p)) != NULL)
	{
		if (dirp->d_type == DT_DIR)
		//std::cout << "dir: " << dirp->d_name << std::endl;
			_file_names.push_back(dirp->d_name);
	}

	closedir(p);

	return true;
}

void FileDealer::PrintStringList(const std::vector<std::string>& _file_names)
{
	for (int i = 0, i_end = (int)_file_names.size(); i < i_end; ++i)
	{
		std::cout << _file_names[i] << std::endl;
	}
}

bool FileDealer::LsFile(std::string dir_name, std::vector<std::string>& file_names)
{
	file_names.clear();
	DIR *p;
	struct dirent *dirp;
	if ((p = opendir(dir_name.c_str())) == NULL)
		return false;

	while ((dirp = readdir(p)) != NULL)
	{
		if (dirp->d_type != DT_DIR)
			file_names.push_back(dirp->d_name);
	}

	closedir(p);

	return true;
}

void FileDealer::PrintFileNames(std::vector<std::string>& file_names)
{
	const int n_files = (int)file_names.size();
	printf("\n");
	for (int i = 0; i < n_files; ++i)
		printf("%d/%d : %s\n", i, n_files, file_names[i].c_str());
}