#include "Tester.h"

#include <time.h>
#include <fstream>
#include <iostream>

#include "CornerDetectorBase.h"
#include "ContourDetector.h"
#include "Commander.h"
#include <cassert>

static void ConvertPointFromCv(
	vector<vector<cv::Point> >& vvpoint_cv,
	vector<vector<CornerDetectorBase::CPoint> >& vvpoint);
static void drawContours(
	int rows,
	int cols,
	const vector<vector<CornerDetectorBase::CPoint> >& _vvpoint,
	const vector<vector<CornerDetectorBase::CPointInfo> >& _vvinfo,
	string save_name);

void Tester::test(
	string& path_input,
	vector<string>& list,
	int id_head,
	int id_back,
	CornerDetectorBase* classifier,
	string& path_oputput)
{
	for (int i = 0; i < (int)list.size(); ++i)
	{
		if (i<id_head || i>id_back) continue;
		cout << i << " / " << (int)list.size() << endl;
		clock_t start_time = clock();
		string full_name_input = path_input + list[i];
		string full_name_output = path_oputput + list[i];
		testOne(full_name_input, classifier, full_name_output);
		cout << "  Total Time : " << clock() - start_time << " ms" << endl;
	}
}

void Tester::testOne(
	string name_input,
	CornerDetectorBase* classifier,
	string name_output)
{
	//read image
	clock_t start_time = clock();
	Mat img = imread(name_input, 0);//gray image;
	assert(!img.empty());
	cout << "\t" << name_input << endl;
	cout << "\tread time : " << clock() - start_time << " ms" << endl;

	//contour detect
	start_time = clock();
	vector<vector<cv::Point> > vvpoint_cv;
	ContourDetector* contour_detector = new ContourDetector();
	contour_detector->detect(img, vvpoint_cv);
	cout << "\tcontour detect time : " << clock() - start_time << " ms" << endl;

	//contour classify 
	start_time = clock();
	vector<vector<CornerDetectorBase::CPoint> > vvpoint;
	vector<vector<CornerDetectorBase::CPointInfo> > vvinfo;
	ConvertPointFromCv(vvpoint_cv, vvpoint);
	classifier->detect(vvpoint, vvinfo);
	cout << "\tcontour classify  time : " << clock() - start_time << " ms" << endl;

	//save
	drawContours(img.rows, img.cols, vvpoint, vvinfo, name_output);
	cout << "\tsave time : " << clock() - start_time << " ms" << endl;
}
void ConvertPointFromCv(
	vector<vector<cv::Point> >& vvpoint_cv,
	vector<vector<CornerDetectorBase::CPoint> >& vvpoint)
{
	//check input, output
	vvpoint.resize(vvpoint_cv.size());
	for (vector<int>::size_type i = 0; i < vvpoint_cv.size(); ++i)
	{
		vvpoint[i].resize(vvpoint_cv[i].size());
		for (vector<int>::size_type j = 0; j < vvpoint_cv[i].size(); ++j)
		{
			vvpoint[i][j].x = vvpoint_cv[i][j].x;
			vvpoint[i][j].y = vvpoint_cv[i][j].y;
		}
	}
}
static void drawContour(
	Mat& _img,
	const std::vector<CornerDetectorBase::CPoint>& _vpoint,
	const std::vector<CornerDetectorBase::CPointInfo>& _vinfo)
{
	assert(_img.type() == CV_8UC3);
	const int n_points = (int)_vpoint.size();
	for (int p = 0; p < n_points; ++p)
	{
		switch (_vinfo[p].type)
		{
		case CornerDetectorBase::CPointType::UNKOWN:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 255, 255);
			break;
		case CornerDetectorBase::CPointType::CIRCLE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 0, 100);
			break;
		case CornerDetectorBase::CPointType::FULL_CIRCLE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 255, 0);
			break;
		case CornerDetectorBase::CPointType::LINE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(100, 255, 100);
			break;
		}
		if (_vinfo[p].corner)
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(0, 0, 255);
		if (_vinfo[p].curv_corner)
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(100, 100, 255);
		if (_vinfo[p].qie_dian)
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(0, 255, 255);
	}
}

void drawContours(
	int rows,
	int cols,
	const vector<vector<CornerDetectorBase::CPoint> >& _vvpoint,
	const vector<vector<CornerDetectorBase::CPointInfo> >& _vvinfo,
	string save_name)
{
	Mat img = Mat::zeros(rows, cols, CV_8UC3);

	const int n_contours = (int)_vvpoint.size();
	for (int c = 0; c <n_contours; ++c)
		drawContour(img, _vvpoint[c], _vvinfo[c]);

	imwrite(save_name, img);
}