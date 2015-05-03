#include "ContourDetector.h"

void ContourDetector::thresholdSimple(cv::Mat& _img)
{
	assert(_img.type() == CV_8UC1);
	for (int r = 0; r < _img.rows; ++r)
	{
		for (int c = 0; c < _img.cols; ++c)
		{
			if (_img.at<unsigned char>(r, c)>128)
				_img.at<unsigned char>(r, c) = 255;
			else
				_img.at<unsigned char>(r, c) = 0;
		}
	}
}

void ContourDetector::detect(cv::Mat _img, std::vector<std::vector<cv::Point> >& _contours)
{
	assert(_img.type() == CV_8UC1);
	thresholdSimple(_img);
	std::vector<cv::Vec4i> hierarchy;
	cv::Mat img = _img.clone();
	cv::findContours(img, _contours, hierarchy, cv::RETR_LIST, cv::CHAIN_APPROX_NONE);
}