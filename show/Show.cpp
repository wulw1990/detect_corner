#include "Show.h"

void Show::DrawContour(
	cv::Mat& _img,
	const std::vector<ContourClassifierBase::Point>& _vpoint,
	const std::vector<ContourClassifierBase::PointInfo>& _vinfo)
{
	assert(_img.type() == CV_8UC3);
	const int n_points = (int)_vpoint.size();
	for (int p = 0; p < n_points; ++p)
	{
		switch (_vinfo[p].type)
		{
		case ContourClassifierBase::PointType::UNKOWN:
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(255, 255, 255);
			break;
		case ContourClassifierBase::PointType::CIRCLE:
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(255, 0, 100);
			break;
		case ContourClassifierBase::PointType::FULL_CIRCLE:
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(255, 255, 0);
			break;
		case ContourClassifierBase::PointType::LINE:
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(100, 255, 100);
			break;
		}
		if (_vinfo[p].corner)
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(100, 100, 255);
		if (_vinfo[p].curv_corner)
			_img.at<cv::Vec3b>(_vpoint[p].y, _vpoint[p].x) = cv::Vec3b(100, 100, 255);
	}
}

void Show::DrawContours(
	cv::Size& _size,
	const std::vector<std::vector<ContourClassifierBase::Point> >& _vvpoint,
	const std::vector<std::vector<ContourClassifierBase::PointInfo> >& _vvinfo,
	const std::string& name)
{
	cv::Mat img = cv::Mat::zeros(_size, CV_8UC3);

	const int n_contours = (int)_vvpoint.size();
	for (int c = 0; c <n_contours; ++c)
		DrawContour(img, _vvpoint[c], _vvinfo[c]);

	cv::imwrite(name.c_str(), img);
}