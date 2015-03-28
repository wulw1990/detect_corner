#ifndef _CONTOUR_CLASSIFIER_CURV_H_WULIWEI_BBNC_TSINGHUA_
#define _CONTOUR_CLASSIFIER_CURV_H_WULIWEI_BBNC_TSINGHUA_

#include "ContourClassifierBase.h"

class ContourClassifierCurv : public ContourClassifierBase
{
public:
	void Classify(
		const std::vector<std::vector<Point> >& _vvpoint,
		std::vector<std::vector<PointInfo> >& _vvinfo);

	ContourClassifierCurv(double _t);
private:
	int r_;
	double thresh_curv;

	void ClassifyEach(
		const std::vector<Point>& _vpoint,
		std::vector<PointInfo>& _vinfo);

	double TriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr);
};


#endif