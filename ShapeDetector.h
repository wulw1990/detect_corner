#pragma once
#include "CornerDetectorBase.h"
#include <cassert>

typedef CornerDetectorBase::CPoint CPoint;
typedef CornerDetectorBase::CPoint2d CPoint2d;
typedef CornerDetectorBase::CPointInfo CPointInfo;
typedef CornerDetectorBase::CPointType CPointType;

class ShapeDetector
{
public:
	//detect
	//功能：对某一个轮廓中从i_head还是，最长len_max的部分进行形状检测
	//简介：主要是采用二分法，具体判断哪一种形状，则由子类决定（子类实现了不同的isValid和setInfo）
	int detect(vector<CPoint>& vp, int i_head, int len_max, vector<CPointInfo>& vi);
	virtual void merge(vector<CPoint>& vp, vector<CPointInfo>& vi) = 0;
	static void resetInfo(vector<CPointInfo>& vi, int ip);
protected:
	virtual bool isValid(vector<CPoint>& vp, int i_head, int len) = 0;
	virtual void setInfo(vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi) = 0;
	
	bool checkSectionError(vector<double>& error_vec, double max_sec_error, int sec_len = 10);
	bool checkEndError(vector<double>& error_vec, double max_sec_error, int sec_len = 10);
	bool checkErrorToLength(
		int length,
		double error,
		vector<int>& length_thresh,
		vector<double>& error_thresh);//不同的长度对应不同的误差阈值

	//inline
	template<typename T> T getMax(vector<T>& data)
	{
		assert((int)data.size() > 0);
		T result = data[0];
		for (int i = 0; i < (int)data.size(); ++i)
			if (data[i]>result) result = data[i];
		return result;
	}
	static int getIndex(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
	static double getTriangleHeight(CPoint2d& pl, const CPoint2d& p, const CPoint2d& pr)
	{
		double di = abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
		di /= sqrt(pow(pr.x - pl.x, 2) + pow(pr.y - pl.y, 2));
		return di;
	}
	static double getDistance(CPoint2d p1, CPoint2d p2)
	{
		double d = pow(p1.x - p2.x, 2) + pow(p1.y - p2.y, 2);
		return sqrt(d + 1e-8);
	}
};
