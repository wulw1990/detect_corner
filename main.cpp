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
#include "CornerDetectorBase.h"
#include "CornerDetectorFit.h"
#include "CornerDetectorCurv.h"
#include "Tester.h"
#include "Commander.h"

using namespace std;
using namespace cv;

int main()
{
#if 0
	clock_t start_time = clock();
	Mat img = imread("D:\\No_Backup_BBNC\\LingYun\\\BMP_Gerber_zhao_big\\l1_big.bmp");
	cout << img.size() << endl;
	cout << "Read time : " << clock() - start_time << " ms" << endl;
#else
	string path_input = ".\\image\\";
	string path_output = ".\\output\\";

	//get input name list
	Commander* cmd = new Commander();
	cout << "pwd : " << cmd->pwd() << endl;
	vector<string> list = cmd->ls(path_input);
	cmd->printList(list);

	//test
	CornerDetectorBase* classifier = new CornerDetectorFit();
	//CornerDetectorBase* classifier = new CornerDetectorCurv(0.38f);
	Tester* tester = new Tester();
	int id_head = 0;
	int id_back = 10;
	tester->test(path_input, list, id_head, id_back, classifier, path_output);
#endif
	system("pause");
}