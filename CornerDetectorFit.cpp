#include "CornerDetectorFit.h"
#include <iostream>
#include <cassert>

#define DEBUG 1

void CornerDetectorFit::detect(
	vector<vector<CPoint> >& vvp,
	vector<vector<CPointInfo> >& vvi)
{
	resizeToSame(vvp, vvi);
	for (int c = 0; c < (int)vvp.size(); ++c)
	{
		if ((int)vvp[c].size() <= 5) continue;//过滤掉太小的圆弧
		if (circle_dealer->detectFullCircle(vvp[c], vvi[c])) continue;//检测完整圆弧
		detectSegment(vvp[c], vvi[c]);//检测线段
		detectCircleAfterSegment(vvp[c], vvi[c]);//检测圆弧
		line_dealer->merge(vvp[c], vvi[c]);//线段的合并和去除短线段
		setUnknown(vvi[c]);//将不确定部分置为未知
		setHighCurvature(vvp[c], vvi[c], 3, 0.38);//对位置曲线部分进行拐点检测
		mergeNeignborCorner(vvi[c]);//合并拐点
		checkNeignborCorner(vvi[c]);//合并拐点
		setQieDian(vvi[c]);//设置切点
	}
}
void CornerDetectorFit::detectSegment(
	vector<CPoint>& vp,
	vector<CPointInfo>& vi)
{
	int n_points = (int)vp.size();
	for (int i = 0; i < (int)n_points;)
	{
		bool flag_success = false;
		int len_max = line_dealer->getLenThreshMax();
		i += line_dealer->detect(vp, i, len_max, vi);
	}
}

void CornerDetectorFit::detectCircleAfterSegment(
	vector<CPoint>& vp,
	vector<CPointInfo>& vi)
{
	int n_points = (int)vp.size();
	for (int i_head = 0; i_head < (int)n_points; ++i_head)
	{
		if (line_dealer->isLineSure(vi, i_head)) continue;
		int cirlength_max = circle_dealer->getLenThreshMax();

		int i_back = i_head + 1;
		int max_length_small_seg = 0;
		for (int i_back_max = i_head + cirlength_max - 1; i_back <= i_back_max; ++i_back)
		{
			if (line_dealer->isLineSure(vi, i_back))
				break;
		}
		i_back -= 1;//最后一个符合条件的点
		int len_max = i_back - i_head + 1;
		
		int len = circle_dealer->detect(vp, i_head, len_max, vi);
		i_head = i_head + len - 1;
	}
}
void CornerDetectorFit::setUnknown(vector<CPointInfo>& vi)
{
	int N = (int)vi.size();
	int min_length = 30;
	for (int i = 0; i < N; ++i)
	{
		if (vi[i].type != CPointType::UNKOWN && vi[i].len < min_length)
			vi[i].Reset();
	}
}
double getTriangleHeight(CPoint2d& pl, CPoint2d& p, CPoint2d& pr)
{
	double di = abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= sqrt(pow(pr.x - pl.x, 2) + pow(pr.y - pl.y, 2));
	return di;
}
void CornerDetectorFit::setHighCurvature(vector<CPoint>& vp, vector<CPointInfo>& vi, int R, double threshcurv)
{
	//int R = 3;
	//double threshcurv = 0.38f;
	int N = (int)vp.size();
	if (N < (R * 2 + 1)) return;
	vector<double> thetas(N);
	for (int i = 0; i < N; ++i)
	{
		if (vi[i].type != CPointType::UNKOWN){
			thetas[i] = 0;
			continue;
		}
		CPoint2d p(vp[getIndex(i, N)]);
		CPoint2d pl(vp[getIndex(i - R, N)]);
		CPoint2d pr(vp[getIndex(i + R, N)]);

		double h = getTriangleHeight(pl, p, pr);
		thetas[i] = asin(h / R);
	}
	for (int i = 0; i < N; ++i)
	{
		int i_pre = getIndex(i - 1, N);
		int i_nex = getIndex(i + 1, N);

		if (thetas[i]>threshcurv && thetas[i] > thetas[i_pre] && thetas[i] > thetas[i_nex]
			&& !vi[i_pre].corner && !vi[i_nex].corner)
			vi[i].curv_corner = true;
	}
}
void CornerDetectorFit::mergeNeignborCorner(vector<CPointInfo>& vi)
{
	const int N = (int)vi.size();
	for (int i = 0; i < N; ++i){
		int i_nex = getIndex(i + 1, N);
		if (vi[i].corner && vi[i_nex].corner){
			if (vi[i].type == CPointType::LINE)
				vi[i_nex].corner = false;
			else
				vi[i].corner = false;
		}
	}
}
void CornerDetectorFit::checkNeignborCorner(vector<CPointInfo>& vi)
{
	const int N = (int)vi.size();
	for (int i = 0; i < N; ++i){
		int i_nex = getIndex(i + 1, N);
		assert(!(vi[i].corner && vi[i_nex].corner));
		assert(!(vi[i].corner && vi[i_nex].curv_corner));
		assert(!(vi[i].curv_corner && vi[i_nex].corner));
	}
}
void CornerDetectorFit::setQieDian(vector<CPointInfo>& vi)
{
	const int N = (int)vi.size();
	for (int i = 0; i < N; ++i){
		int i_pre = getIndex(i - 1, N);
		int i_nex = getIndex(i + 1, N);
		if (!vi[i].corner) continue;
		if (vi[i].type == CPointType::LINE){
			if (vi[i_pre].type == CPointType::CIRCLE || vi[i_nex].type == CPointType::CIRCLE)
				vi[i].qie_dian = true;
		}
		else if (vi[i].type == CPointType::CIRCLE){
			if (vi[i_pre].type == CPointType::LINE || vi[i_nex].type == CPointType::LINE)
				vi[i].qie_dian = true;
		}
	}
}