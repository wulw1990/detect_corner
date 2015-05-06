#pragma once

#include "CornerDetectorBase.h"

class CornerDetectorCurv : public CornerDetectorBase
{
public:
	void detect(
		std::vector<std::vector<CPoint> >& _vvpoint,
		std::vector<std::vector<CPointInfo> >& _vvinfo);

	CornerDetectorCurv(double _t);
private:
	int r_;
	double thresh_curv;

	void classifyEach(
		const std::vector<CPoint>& _vpoint,
		std::vector<CPointInfo>& _vinfo);

	double getTriangleHeight(const CPoint2d& pl, const CPoint2d& p, const CPoint2d& pr);
};