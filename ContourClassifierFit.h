#ifndef _CONTOUR_CLASSIFIER_FIT_H_WULIWEI_BBNC_TSINGHUA_
#define _CONTOUR_CLASSIFIER_FIT_H_WULIWEI_BBNC_TSINGHUA_

#include "ContourClassifierBase.h"
#include "ContourClassifierFitHelper.h"


class ContourClassifierFit : public ContourClassifierBase
{
public:
	void Classify(
		const std::vector<std::vector<Point> >& _vvpoint,
		std::vector<std::vector<PointInfo> >& _vvinfo);

	ContourClassifierFit(){ 
		line_dealer = new LineDealer();
		circle_dealer = new CircleDealer();
	}

private:
	LineDealer* line_dealer;
	CircleDealer* circle_dealer;

	//--Segment ------------------------------------------------
	void DetectSegment(
		const std::vector<Point>& _c,
		std::vector<struct PointInfo>& _vi);

	void mergeLine(
		const std::vector<Point>& _c,
		std::vector<struct PointInfo>& _vi);

	//--circle --------------------------------------------------
	void DetectCircleAfterSegment(
		const std::vector<Point>& _c,
		std::vector<struct PointInfo>& _vi);

	//-- unknown -----------------------------------------------
	void SetUnknown(std::vector<struct PointInfo>& _vi);



	void SetHighCurvature(const std::vector<Point>& vp, std::vector<PointInfo>& vi, int r_, double thresh_curv);
};


#endif