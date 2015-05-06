#pragma once
#include "CornerDetectorBase.h"

typedef CornerDetectorBase::CPoint CPoint;
typedef CornerDetectorBase::CPoint2d CPoint2d;
typedef CornerDetectorBase::CPointInfo CPointInfo;
typedef CornerDetectorBase::CPointType CPointType;

class ShapeDealer
{
public:
	//detect
	//功能：对某一个轮廓中从i_head还是，最长len_max的部分进行形状检测
	//简介：主要是采用二分法，具体判断哪一种形状，则由子类决定（子类实现了不同的isValid和setInfo）
	int detect(
		const vector<CPoint>& vp, int i_head, int len_max,
		vector<CPointInfo>& vi);
	virtual void merge(const vector<CPoint>& vp, vector<CPointInfo>& vi) = 0;
	virtual void setUnknown(vector<CPointInfo>& vi) = 0;
	static void resetInfo(vector<CPointInfo>& vi, int ip);
protected:
	virtual bool isValid(const vector<CPoint>& vp, int i_head, int len) = 0;
	virtual void setInfo(const vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi) = 0;

	
	static int index(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
	double getTriangleHeight(const CPoint2d& pl, const CPoint2d& p, const CPoint2d& pr);
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
	void merge(const vector<CPoint>& vp, vector<CPointInfo>& vi);
	bool isLineSure(const vector<CPointInfo>& vi, int i);
	int getLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
	void setUnknown(vector<CPointInfo>& vi);
private:
	int len_sure;

	vector<int> length_thresh;
	vector<double> error_thresh;

	bool isValid(const vector<CPoint>& vp, int i_head, int len);
	void setInfo(const vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi);
	bool getErrorVec(const vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec);

};

class CircleDealer : public ShapeDealer
{
public:
	CircleDealer();
	bool detectFullCircle(const vector<CPoint>& vp, vector<CPointInfo>& vi);
	void merge(const vector<CPoint>& vp, vector<CPointInfo>& vi);
	void completeCircle(
		const vector<CPoint>& _c,
		vector<CPointInfo>& _vi);
	int getLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
	void setUnknown(vector<CPointInfo>& vi);
private:
	double radius_min;
	double radius_max;
	double radian_min;
	CPoint2d center_;
	double radius_;

	vector<int> length_thresh;
	vector<double> error_thresh;

	bool isValid(const vector<CPoint>& vp, int i_head, int len);
	void setInfo(const vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi);
	bool getErrorVec(const vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec);

	static void getCircle(
		const vector<CPoint>& vp, int i_head, int len, CPoint2d& center, double& radius);
	static void getCircle(
		const CPoint2d& p1, const CPoint2d& p2, const CPoint2d& p3, CPoint2d& _center, double& _radius);
	static void getCircleLinear(
		const vector<CPoint>& vp, int i_head, int len, CPoint2d& center, double& radius);
	static bool getErrorVec(const std::vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec
		, CPoint2d& center, double& radius);
	static double getError(CPoint2d& p, CPoint2d& center, double& radius)
	{
		double dis_sqaure= (center.x - p.x)*(center.x - p.x) + (center.y - p.y)*(center.y - p.y);
		double dis = sqrt(dis_sqaure + 1e-8);
		return abs(dis - radius);
	}

	static void fitCircleLinear(vector<CPoint> points, CPoint2d& center, double& radius);
};