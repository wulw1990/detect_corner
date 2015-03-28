#ifndef _SHOW_H_
#define _SHOW_H_

#include "opencv2/opencv.hpp"
#include "../ContourClassifierBase.h"

namespace Show
{
	void DrawContour(
		cv::Mat& _img,
		const std::vector<ContourClassifierBase::Point>& _vpoint,
		const std::vector<ContourClassifierBase::PointInfo>& _vinfo);

	void DrawContours(
		cv::Size& _size,
		const std::vector<std::vector<ContourClassifierBase::Point> >& _vvpoint,
		const std::vector<std::vector<ContourClassifierBase::PointInfo> >& _vvinfo,
		const std::string& name);

}

#endif