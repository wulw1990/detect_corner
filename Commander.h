#ifndef _FILE_DEALER_H_
#define _FILE_DEALER_H_

#include <string>
#include <vector>

using namespace std;

class Commander
{
public:
	vector<string> ls(string path);
	void printList(vector<string>& list);
	void sortFileList(vector<string>& list);
	void mkdirRecursively(string path);
	string pwd();
	vector<string> readLines(string name);
private:
	string exec(string cmd);
};





#endif