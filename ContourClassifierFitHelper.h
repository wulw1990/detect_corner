#ifndef _CONTOUR_CLASSIFIER_FIT_HELPER_H_WULIWEI_BBNC_TSINGHUA_
#define _CONTOUR_CLASSIFIER_FIT_HELPER_H_WULIWEI_BBNC_TSINGHUA_

#include "ContourClassifierBase.h"

typedef ContourClassifierBase::Point Point;
typedef ContourClassifierBase::Point2d Point2d;
typedef ContourClassifierBase::PointInfo PointInfo;
typedef ContourClassifierBase::PointType PointType;

class ShapeDealer
{
public:
	//return len
	int Detect(
		const std::vector<Point>& vp, int i_head, int len_max,
		std::vector<PointInfo>& vi);
	int GetLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
	virtual void merge(const std::vector<Point>& vp, std::vector<PointInfo>& vi) = 0;
	static void resetInfo(std::vector<PointInfo>& vi, int ip);
	
protected:
	std::vector<int> length_thresh;
	std::vector<double> error_thresh;

	virtual bool isValid(const std::vector<Point>& vp, int i_head, int len) = 0;
	virtual std::vector<double> getErrorVec(const std::vector<Point>& vp, int i_head, int len) = 0;
	virtual void setInfo(const std::vector<Point>& vp, int i_head, int len, std::vector<PointInfo>& vi) = 0;

	
	static int index(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
	double TriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr);
	double vecMax(std::vector<double> vec);
	double secAverageMax(std::vector<double> vec, int sec_len);//分段平均（误差）
	double secAverageMaxEnd(std::vector<double> vec, int sec_len);//端点平均（误差）
	bool secCheck(
		int length,
		double error,
		const std::vector<int>& length_thresh,
		const std::vector<double>& error_thresh);//不同的长度对应不同的误差阈值
};

class LineDealer : public ShapeDealer
{
public:
	LineDealer();
	int GetLenSure(){ return len_sure; }
	void merge(const std::vector<Point>& vp, std::vector<PointInfo>& vi);
	void removeShort(std::vector<PointInfo>& vi);
	bool isLineSure(const std::vector<PointInfo>& vi, int i);
private:
	int len_sure;

	bool isValid(const std::vector<Point>& vp, int i_head, int len);
	std::vector<double> getErrorVec(const std::vector<Point>& vp, int i_head, int len);
	void setInfo(const std::vector<Point>& vp, int i_head, int len, std::vector<PointInfo>& vi);
};

class CircleDealer : public ShapeDealer
{
public:
	CircleDealer();
	bool detectFullCircle(const std::vector<Point>& vp, std::vector<PointInfo>& vi);
	void merge(const std::vector<Point>& vp, std::vector<PointInfo>& vi);
	void completeCircle(
		const std::vector<Point>& _c,
		std::vector<PointInfo>& _vi);
private:
	double radius_min;
	double radius_max;
	double radian_min;
	Point2d center_;
	double radius_;

	bool isValid(const std::vector<Point>& vp, int i_head, int len);
	std::vector<double> getErrorVec(const std::vector<Point>& vp, int i_head, int len);
	void setInfo(const std::vector<Point>& vp, int i_head, int len, std::vector<PointInfo>& vi);

	static void getCircle(
		const std::vector<Point>& vp, int i_head, int len, Point2d& center, double& radius);
	static void getCircle(
		const Point2d& p1, const Point2d& p2, const Point2d& p3, Point2d& _center, double& _radius);
	static void getCircleLinear(
		const std::vector<Point>& vp, int i_head, int len, Point2d& center, double& radius);
	static std::vector<double> getErrorVec(const std::vector<Point>& vp, int i_head, int len
		, Point2d& center, double& radius);
	static double getError(Point2d& p, Point2d& center, double& radius)
	{
		double dis_sqaure= (center.x - p.x)*(center.x - p.x) + (center.y - p.y)*(center.y - p.y);
		if (dis_sqaure < 1e-8)
			dis_sqaure = 1e-8;
		double dis = std::sqrt(dis_sqaure);
		return std::abs(dis - radius);
	}

	static void fitCircleLinear(std::vector<Point> points, Point2d& center, double& radius);
};


#endif