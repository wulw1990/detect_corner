#ifndef _FILE_DEALER_H_
#define _FILE_DEALER_H_

#include <string>
#include <vector>

using namespace std;

class FileDealer
{
public:
	vector<string> ls(string path);
	void print_list(vector<string>& list);
	void sortFileList(vector<string>& list);
	void mkdirRecursively(string path);
	string pwd();

private:
	std::string FileDealer::exec(string cmd);
};





#endif