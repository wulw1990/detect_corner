#pragma once
#include "CornerDetectorBase.h"
#include "CircleDetector.h"
#include "LineDetector.h"

class CornerDetectorFit : public CornerDetectorBase
{
public:
	//detect
	//由外部调用的方法
	//实现基类CornerDetectorBase中的纯虚函数
	//具体说明见CornerDetectorBase.h
	void detect(
		vector<vector<CPoint> >& vvp,
		vector<vector<CPointInfo> >& vvi);

	//setThresh
	//功能：自定义阈值
	//输入：
	//	circle_error_thresh_short -- 短圆弧的最大误差（小于20个像素）
	//	circle_error_thresh_long -- 长圆弧的最大误差
	//	circle_radius_min -- 圆弧的最小半径
	//	circle_radius_max -- 圆弧的最大半径
	//	circle_radian_min -- 圆弧的最小弧度（单位是读，默认是60度）
	void setThresh(
		double circle_error_thresh_short = 0.6,
		double circle_error_thresh_long = 1.0,
		double circle_radius_min = 5,
		double circle_radius_max = 100,
		double circle_radian_min = 60)
	{
		circle_dealer->setThresh(
			circle_error_thresh_short,
			circle_error_thresh_long,
			circle_radius_min,
			circle_radius_max,
			circle_radian_min);
	}

	CornerDetectorFit(){
		line_dealer = new LineDetector();
		circle_dealer = new CircleDetector();
	}
	~CornerDetectorFit(){
		delete line_dealer;
		delete circle_dealer;
	}

private:
	//LineDealer和CircleDealer的定义在CornerDetectorFitHelper中
	//功能：协助处理线段和圆弧
	LineDetector* line_dealer;
	CircleDetector* circle_dealer;

	//detectSegment
	//功能：对一个轮廓进行线段检测
	//输入：vp -- 一个轮廓对应的所有点
	//输出：vi -- 与vp对应的每个点的信息
	void detectSegment(
		vector<CPoint>& vp,
		vector<CPointInfo>& vi);

	//detectCircleAfterSegment
	//功能：在检测完线段后，进行圆弧检测
	//输入：vp -- 一个轮廓对应的所有点
	//输出：vi -- 与vp对应的每个点的信息
	void detectCircleAfterSegment(
		vector<CPoint>& vp,
		vector<CPointInfo>& vi);

	//setUnknown
	//功能：检测完后，将短的部分置为未知曲线
	void setUnknown(vector<CPointInfo>& vi);

	//setHighCurvature
	//功能：未知曲线部分检测拐点
	//输入：vp -- 一个轮廓对应的所有点
	//输出：vi -- 与vp对应的每个点的信息
	//其他参数（输入）：R -- 计算曲率时使用的半径，threshcurv -- 阈值
	void setHighCurvature(vector<CPoint>& vp, vector<CPointInfo>& vi, int R, double threshcurv);

	//setQieDian
	//功能：设置切点
	//输入：vi -- 与vp对应的每个点的信息
	//输出：vi -- 与vp对应的每个点的信息
	void setQieDian(vector<CPointInfo>& vi);
}; 