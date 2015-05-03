#pragma once
#include "CornerDetectorBase.h"

typedef CornerDetectorBase::Point Point;
typedef CornerDetectorBase::Point2d Point2d;
typedef CornerDetectorBase::PointInfo PointInfo;
typedef CornerDetectorBase::PointType PointType;

class ShapeDealer
{
public:
	//return len
	int detect(
		const vector<Point>& vp, int i_head, int len_max,
		vector<PointInfo>& vi);
	int getLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
	virtual void merge(const vector<Point>& vp, vector<PointInfo>& vi) = 0;
	static void resetInfo(vector<PointInfo>& vi, int ip);
	
protected:
	vector<int> length_thresh;
	vector<double> error_thresh;

	virtual bool isValid(const vector<Point>& vp, int i_head, int len) = 0;
	virtual vector<double> getErrorVec(const vector<Point>& vp, int i_head, int len) = 0;
	virtual void setInfo(const vector<Point>& vp, int i_head, int len, vector<PointInfo>& vi) = 0;

	
	static int index(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
	double getTriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr);
	double vecMax(vector<double> vec);
	double secAverageMax(vector<double> vec, int sec_len);//分段平均（误差）
	double secAverageMaxEnd(vector<double> vec, int sec_len);//端点平均（误差）
	bool secCheck(
		int length,
		double error,
		const vector<int>& length_thresh,
		const vector<double>& error_thresh);//不同的长度对应不同的误差阈值
};

class LineDealer : public ShapeDealer
{
public:
	LineDealer();
	int getLenSure(){ return len_sure; }
	void merge(const vector<Point>& vp, vector<PointInfo>& vi);
	void removeShort(vector<PointInfo>& vi);
	bool isLineSure(const vector<PointInfo>& vi, int i);
private:
	int len_sure;

	bool isValid(const vector<Point>& vp, int i_head, int len);
	vector<double> getErrorVec(const vector<Point>& vp, int i_head, int len);
	void setInfo(const vector<Point>& vp, int i_head, int len, vector<PointInfo>& vi);
};

class CircleDealer : public ShapeDealer
{
public:
	CircleDealer();
	bool detectFullCircle(const vector<Point>& vp, vector<PointInfo>& vi);
	void merge(const vector<Point>& vp, vector<PointInfo>& vi);
	void completeCircle(
		const vector<Point>& _c,
		vector<PointInfo>& _vi);
private:
	double radius_min;
	double radius_max;
	double radian_min;
	Point2d center_;
	double radius_;

	bool isValid(const vector<Point>& vp, int i_head, int len);
	vector<double> getErrorVec(const vector<Point>& vp, int i_head, int len);
	void setInfo(const vector<Point>& vp, int i_head, int len, vector<PointInfo>& vi);

	static void getCircle(
		const vector<Point>& vp, int i_head, int len, Point2d& center, double& radius);
	static void getCircle(
		const Point2d& p1, const Point2d& p2, const Point2d& p3, Point2d& _center, double& _radius);
	static void getCircleLinear(
		const vector<Point>& vp, int i_head, int len, Point2d& center, double& radius);
	static vector<double> getErrorVec(const vector<Point>& vp, int i_head, int len,
		Point2d& center, double& radius);
	static double getError(Point2d& p, Point2d& center, double& radius)
	{
		double dis_sqaure= (center.x - p.x)*(center.x - p.x) + (center.y - p.y)*(center.y - p.y);
		double dis = sqrt(dis_sqaure + 1e-8);
		return abs(dis - radius);
	}

	static void fitCircleLinear(vector<Point> points, Point2d& center, double& radius);
};