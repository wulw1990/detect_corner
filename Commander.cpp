#include "Commander.h"
#include <cassert>
#include <algorithm>
#include <sstream>
#include <string>
#include <iostream>
#include <fstream>
#include <stdio.h>

vector<string> Commander::ls(string path)
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

void Commander::printList(vector<string>& list)
{
	cout << "list : "<< endl;
	for (int i = 0; i < (int)list.size(); ++i)
		cout << i + 1 << "\t" << "--" << list[i] << "--"  << endl;
}

void Commander::mkdirRecursively(string path)
{
	string cmd = string("mkdir ") + path;
	exec(cmd);
}
string Commander::pwd()
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

vector<string> Commander::readLines(string name)
{
	std::ifstream fin(name.c_str());
	std::vector<std::string> lines;
	std::string line;
	while (getline(fin,line))
		lines.push_back(line);
	return lines;
}

void Commander::sortFileList(vector<string>& list)
{
	sort(list.begin(), list.end(), lessFile);
}


//Replace popen and pclose with _popen and _pclose for Windows.
string Commander::exec(string cmd) 
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