#include "CornerDetectorBase.h"

#include "opencv2\opencv.hpp"
using namespace cv;

static void drawContour(
	Mat& _img,
	const std::vector<CornerDetectorBase::Point>& _vpoint,
	const std::vector<CornerDetectorBase::PointInfo>& _vinfo)
{
	assert(_img.type() == CV_8UC3);
	const int n_points = (int)_vpoint.size();
	for (int p = 0; p < n_points; ++p)
	{
		switch (_vinfo[p].type)
		{
		case CornerDetectorBase::PointType::UNKOWN:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 255, 255);
			break;
		case CornerDetectorBase::PointType::CIRCLE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 0, 100);
			break;
		case CornerDetectorBase::PointType::FULL_CIRCLE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(255, 255, 0);
			break;
		case CornerDetectorBase::PointType::LINE:
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(100, 255, 100);
			break;
		}
		if (_vinfo[p].corner)
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(0, 0, 255);
		if (_vinfo[p].curv_corner)
			_img.at<Vec3b>(_vpoint[p].y, _vpoint[p].x) = Vec3b(100, 100, 255);
	}
}

void CornerDetectorBase::drawContours(
	int rows,
	int cols,
	const vector<vector<CornerDetectorBase::Point> >& _vvpoint,
	const vector<vector<CornerDetectorBase::PointInfo> >& _vvinfo,
	string save_name)
{
	Mat img = Mat::zeros(rows, cols, CV_8UC3);

	const int n_contours = (int)_vvpoint.size();
	for (int c = 0; c <n_contours; ++c)
		drawContour(img, _vvpoint[c], _vvinfo[c]);

	imwrite(save_name, img);
}