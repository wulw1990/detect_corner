#pragma once
#include "ShapeDetector.h"

class CircleDetector : public ShapeDetector
{
public:
	CircleDetector();
	bool detectFullCircle(vector<CPoint>& vp, vector<CPointInfo>& vi);
	void merge(vector<CPoint>& vp, vector<CPointInfo>& vi){};
	int getLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
	void setThresh(
		double circle_error_thresh_short = 0.6,
		double circle_error_thresh_long = 1.0,
		double circle_radius_min = 5,
		double circle_radius_max = 100,
		double circle_radian_min = 60)
	{
		error_thresh[0] = circle_error_thresh_short;
		error_thresh[error_thresh.size() - 1] = circle_error_thresh_long;
		radius_min = circle_radius_min;
		radius_max = circle_radius_max;
		radian_min = circle_radian_min;
	}
	static void testMatrix();
private:
	struct CircleInfo
	{
		CPoint2d center;
		double radius;
		CircleInfo(){ center.x = -1; center.y = -1; radius = -1; }
	};
	double radius_min;
	double radius_max;
	double radian_min;
	vector<int> length_thresh;
	vector<double> error_thresh;

	bool isValid(vector<CPoint>& vp, int i_head, int len);
	void setInfo(vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi);

	bool checkCircle(vector<CPoint>& vp, int i_head, int len, CircleInfo circle);
	bool checkCircleRadius(double radius);
	bool checkCircleRadian(vector<CPoint>& vp, int i_head, int len, CircleInfo circle);
	bool checkCircleError(vector<CPoint>& vp, int i_head, int len, CircleInfo circle);

	static bool getCircleMulti(vector<CPoint>& vp, int i_head, int len, vector<CircleInfo>& circle_vec);
	static CircleInfo getCircle(vector<CPoint>& vp, int i_head, int len);
	static CircleInfo getCircle(CPoint2d& p1, CPoint2d& p2, CPoint2d& p3);
	static CircleInfo getCircleLinear(vector<CPoint>& vp, int i_head, int len);
	static CircleInfo getCircleLinear(vector<CPoint>& points);

	static bool getCircleErrorVec(
		vector<CPoint>& vp, int i_head, int len, 
		double max_error, 
		vector<double>& error_vec, 
		CircleInfo circle);

	static double getCircleError(CPoint2d& p, CircleInfo circle)
	{
		double cx = circle.center.x;
		double cy = circle.center.y;
		double dis = sqrt((cx - p.x)*(cx - p.x) + (cy - p.y)*(cy - p.y) + 1e-8);
		return abs(dis - circle.radius);
	}

	//needed by getCircleLinear
	static vector<vector<double> > matrixMultiply(vector<vector<double> >A, vector<vector<double> >B);
	static vector<vector<double> > matrixInvert(vector<vector<double> >A);
	static vector<vector<double> > matrixTrans(vector<vector<double> >A);
};