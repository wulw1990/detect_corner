#ifndef _FILE_DEALER_H_WULIWEI_BBNC_TSINGHUA_
#define _FILE_DEALER_H_WULIWEI_BBNC_TSINGHUA_

#include <string>
#include <vector>
class FileDealer
{
public:
	FileDealer(){}
	int CreatDir(std::string _dir);
	bool GetFileList(std::string _dir, std::vector<std::string>& _file_names);
	void PrintStringList(const std::vector<std::string>& _file_names);
	bool LsFile(std::string dir_name, std::vector<std::string>& file_names);
	void PrintFileNames(std::vector<std::string>& file_names);
};
#endif