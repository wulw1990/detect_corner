#include "opencv2/opencv.hpp"
#define CV_VERSION_ID CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
#ifdef _DEBUG
#define cvLIB(name) "opencv_" name CV_VERSION_ID "d"
#else
#define cvLIB(name) "opencv_" name CV_VERSION_ID
#endif
#pragma comment( lib, cvLIB("core"))
#pragma comment( lib, cvLIB("imgproc"))
#pragma comment( lib, cvLIB("highgui"))


#include <assert.h>
#include <time.h>
#include <algorithm>
#include <fstream>

#include "ContourDetector.h"
#include "ContourClassifierBase.h"
#include "ContourClassifierFit.h"
#include "ContourClassifierCurv.h"
#include "Tester.h"
#include "./system/FileDealer.h"

void FitCircleLinear(std::vector<cv::Point> points, cv::Point2d& center, double& radius);

int main()
{
#if 0
	std::string base_name = "D:\\No_Backup_BBNC\\LingYun\\";
	std::string base_name_output = "D:\\No_Backup_BBNC\\LingYun\\output_corner\\";

	std::string list_name = "name_list.txt";
	ContourClassifierFit* classifier = new ContourClassifierFit();

	Tester* tester = new Tester();
	tester->TestFileList(base_name, list_name, classifier, base_name_output);
	//tester->TestOneInListFile(base_name, list_name, classifier, base_name_output, 0);
#endif
	std::string base_name = ".\\image\\";

	//get input name list
	FileDealer* file_dealer = new FileDealer();
	cout << "pwd : " << file_dealer->pwd() << endl;
	std::vector<std::string> list_name = file_dealer->ls(base_name);
	file_dealer->print_list(list_name);

	std::string base_name_output = ".\\output\\";
	std::vector<std::string> list_name_output(list_name.size());

	for (int i = 0, n_files = (int)list_name.size(); i < n_files; ++i)
	{
		std::string ext = list_name[i].substr(list_name[i].length() - 4, 4);
		std::string name = list_name[i].substr(0, list_name[i].length() - 4);
		list_name_output[i] = name + "_output" + ext;
	}
		
	//test
	ContourClassifierBase* classifier = new ContourClassifierFit();
	//ContourClassifierBase* classifier = new ContourClassifierCurv(0.38f);
	Tester* tester = new Tester();
	tester->TestFileList(base_name, list_name, classifier, base_name_output, list_name_output);

	system("pause");
}