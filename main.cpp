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
#include "./system_dependent/FileDealer.h"

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
	std::vector<std::string> list_name;

	//get input name list
	FileDealer* file_dealer = new FileDealer();
	file_dealer->LsFile(base_name, list_name);
	file_dealer->PrintFileNames(list_name);

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


#if 0
	//test
	cv::Mat img = cv::Mat::zeros(500, 500, CV_8UC3);
	std::vector<cv::Point> points;
	double radius = 200;
	cv::Point2d center(250, 250);
	for (int i = 0; i <= 360; i += 2)
	{
		double theta = (double)i / 180.0 * 3.1415926;
		int x = center.x + std::cos(theta) * radius;
		int y = center.y + std::sin(theta) * radius;
		points.push_back(cv::Point(x, y));
	}
	points.resize(points.size() / 32);

	for (int i = 0; i < (int)points.size(); ++i)
		cv::circle(img, points[i], 3, cv::Scalar(0, 0, 255), -1);

	cv::Point2d center_;
	double radius_;
	FitCircleLinear(points, center_, radius_);
	std::cout << center_ << std::endl;
	std::cout << radius_ << std::endl;

	cv::circle(img, center_, radius_, cv::Scalar(255, 0, 0), 1);

	cv::imshow("img", img);
	cv::waitKey(0);
#endif

	//system("pause");
}


void FitCircleLinear(std::vector<cv::Point> points, cv::Point2d& center, double& radius)
{
	const int n = (int)points.size();
	cv::Mat A(n, 3, CV_64FC1);
	cv::Mat b(3, 1, CV_64FC1);
	cv::Mat Y(n, 1, CV_64FC1);

	for (int i = 0; i < n; ++i)
	{
		A.at<double>(i, 0) = 1;
		A.at<double>(i, 1) = points[i].y;
		A.at<double>(i, 2) = points[i].x;

		Y.at<double>(i, 0) = -points[i].x*points[i].x - points[i].y*points[i].y;
	}

	b = (A.t()*A).inv()*A.t()*Y;
	//std::cout << b << std::endl;
	double AA = b.at<double>(2, 0);
	double BB = b.at<double>(1, 0);
	double CC = b.at<double>(0, 0);

	center.x = AA / (-2.0);
	center.y = BB / (-2.0);
	radius = std::sqrt(center.x*center.x + center.y*center.y - CC);
}




