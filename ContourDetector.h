#ifndef _CONTOUR_DETECTOR_H_WULIWEI_BBNC_TSINGHUA_
#define _CONTOUR_DETECTOR_H_WULIWEI_BBNC_TSINGHUA_

#include "opencv2/opencv.hpp"

class ContourDetector
{
public:
	/*
	* input : _img -- CV_8UC1, binary image (0, 255)
	* output : 
	*	_contours -- all the contours detected
	*	_img -- _img will be binaried, if not binary exactly
	*/
	void Detect(cv::Mat _img, std::vector<std::vector<cv::Point> >& _contours);

private:
	void ThresholdSimple(cv::Mat& _img);
};

#endif