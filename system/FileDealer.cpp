#include "FileDealer.h"
#include <cassert>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <stdio.h>

vector<string> FileDealer::ls(string path)
{
	vector<string> result;
	string cmd = string("dir /B ") + path;
	cout << "cmd : " << cmd << endl;
	string exe_result = exec(cmd);
	//cout << "exe_result : " << exe_result << endl;
	stringstream ss;
	ss << exe_result;
	string str;
	while (ss >> str)
		result.push_back(str);
	return result;
}

void FileDealer::print_list(vector<string>& list)
{
	cout << "list : "<< endl;
	for (int i = 0; i < (int)list.size(); ++i)
		cout << i + 1 << "\t" << "--" << list[i] << "--"  << endl;
}

void FileDealer::mkdirRecursively(string path)
{
	string cmd = string("mkdir ") + path;
	exec(cmd);
}
string FileDealer::pwd()
{
	return exec("chdir");
}

static bool lessFile(string a, string b)
{
	string zero = "0";
	if (a.length() < b.length())
	{
		for (int i = 0; i < (int)(b.length() - a.length()); ++i)
			a = zero + a;
	}
	if (a.length() > b.length())
	{
		for (int i = 0; i < (int)(a.length() - b.length()); ++i)
			b = zero + b;
	}
	return a < b;
}
void FileDealer::sortFileList(vector<string>& list)
{
	sort(list.begin(), list.end(), lessFile);
}


//Replace popen and pclose with _popen and _pclose for Windows.
string FileDealer::exec(string cmd) 
{
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe) return "ERROR";
	char buffer[128];
	string result = "";
	while (!feof(pipe)) {
		if (fgets(buffer, 128, pipe) != NULL)
			result += buffer;
	}
	_pclose(pipe);
	return result;
}