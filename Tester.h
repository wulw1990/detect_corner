#ifndef _TESTER_H_
#define _TESTER_H_

#include <vector>
#include <string>
class ContourClassifierBase;
class FileDealer;

class Tester
{
public:
	Tester();

	void TestFileList(
		const std::string& base_name, 
		const std::string& list_name,
		ContourClassifierBase* classifier,
		const std::string& base_name_output);

	void TestFileList(
		const std::string& base_name,
		const std::vector<std::string>& name_list,
		ContourClassifierBase* classifier,
		const std::string& base_name_output,
		const std::vector<std::string>& name_list_output);

	void TestOneInListFile(
		const std::string& base_name,
		const std::string& list_name,
		ContourClassifierBase* classifier,
		const std::string& base_name_output,
		int num);

private:
	FileDealer* file_dealer;

	void TestFileList(
		const std::string& base_name,
		const std::vector<std::string>& name_list,
		ContourClassifierBase* classifier,
		const std::string& base_name_output);

	void TestOneFile(
		const std::string& full_name,
		ContourClassifierBase* classifier,
		const std::string& full_name_output);

	void TestOneImage(
		const std::string& full_name,
		ContourClassifierBase* classifier,
		const std::string& full_name_output);

	std::vector<std::string> ReadNameList(std::string txt_name);
};

#endif
