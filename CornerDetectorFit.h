#pragma once
#include "CornerDetectorBase.h"
#include "CornerDetectorFitHelper.h"

class CornerDetectorFit : public CornerDetectorBase
{
public:
	void detect(
		vector<vector<Point> >& _vvpoint,
		vector<vector<PointInfo> >& _vvinfo);

	CornerDetectorFit(){
		line_dealer = new LineDealer();
		circle_dealer = new CircleDealer();
	}
	~CornerDetectorFit(){
		delete line_dealer;
		delete circle_dealer;
	}

private:
	LineDealer* line_dealer;
	CircleDealer* circle_dealer;

	void detectSegment(
		vector<Point>& _c,
		vector<struct PointInfo>& _vi);
	void detectCircleAfterSegment(
		vector<Point>& _c,
		vector<struct PointInfo>& _vi);
	void setUnknown(vector<struct PointInfo>& _vi);
	void setHighCurvature(vector<Point>& vp, vector<PointInfo>& vi, int r_, double thresh_curv);
};