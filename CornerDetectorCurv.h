#pragma once

#include "CornerDetectorBase.h"

class CornerDetectorCurv : public CornerDetectorBase
{
public:
	void detect(
		std::vector<std::vector<Point> >& _vvpoint,
		std::vector<std::vector<PointInfo> >& _vvinfo);

	CornerDetectorCurv(double _t);
private:
	int r_;
	double thresh_curv;

	void classifyEach(
		const std::vector<Point>& _vpoint,
		std::vector<PointInfo>& _vinfo);

	double getTriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr);
};