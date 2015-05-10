#include "CornerDetectorFit.h"
#include <iostream>

#define SQUARE(x) ((x)*(x))
#define DEBUG 1

void CornerDetectorFit::detect(
	vector<vector<CPoint> >& vvp,
	vector<vector<CPointInfo> >& vvi)
{
	resizeToSame(vvp, vvi);
	for (int c = 0; c < (int)vvp.size(); ++c)
	{
		//过滤掉太小的圆弧
		if ((int)vvp[c].size() <= 5) continue;

		//检测完整圆弧
		if (circle_dealer->detectFullCircle(vvp[c], vvi[c])) continue;

		//检测线段
		detectSegment(vvp[c], vvi[c]);

		//检测圆弧
		detectCircleAfterSegment(vvp[c], vvi[c]);

		//线段的合并和去除短线段
		line_dealer->merge(vvp[c], vvi[c]);

		//将不确定部分置为未知
		setUnknown(vvi[c]);

		//对位置曲线部分进行拐点检测
		setHighCurvature(vvp[c], vvi[c], 3 , 0.38);

		//设置切点
		setQieDian(vvi[c]);
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
		if (line_dealer->isLineSure(vi, i_head))
			continue;
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
	line_dealer->setUnknown(vi);
	circle_dealer->setUnknown(vi);
}
double getTriangleHeight(CPoint2d& pl, CPoint2d& p, CPoint2d& pr)
{
	if (pl == pr)
		return 0;
	double di = abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= sqrt(SQUARE(pr.x - pl.x) + SQUARE(pr.y - pl.y));
	return di;
}
void CornerDetectorFit::setHighCurvature(vector<CPoint>& vp, vector<CPointInfo>& vi, int R, double threshcurv)
{
	//int R = 3;
	//double threshcurv = 0.38f;
	int n_points = (int)vp.size();
	if (n_points < (R * 2 + 1))
		return;
	vector<double> thetas(n_points);
	for (int ip = 0; ip < n_points; ++ip)
	{
		if (vi[ip].type != CPointType::UNKOWN)
		{
			thetas[ip] = 0;
			continue;
		}
		int ipl = ip - R;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + R;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		CPoint2d p(vp[ip]);
		CPoint2d pl(vp[ipl]);
		CPoint2d pr(vp[ipr]);

		double h = getTriangleHeight(pl, p, pr);
		thetas[ip] = asin(h / R);
	}
	for (int ip = 0; ip < n_points; ++ip)
	{
		int ipl = ip - 1;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + 1;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		if (thetas[ip]>threshcurv && thetas[ip] >= thetas[ipl] && thetas[ip] >= thetas[ipr])
			vi[ip].curv_corner = true;
	}
}
void CornerDetectorFit::setQieDian(vector<CPointInfo>& vi)
{
	const int n = (int)vi.size();
	for (int i = 0; i < n; ++i){
		if (vi[index(i, n)].corner == true && vi[index(i, n)].type == CPointType::LINE && vi[index(i - 1, n)].type == CPointType::CIRCLE)
			vi[index(i, n)].qie_dian = true;
		else if (vi[index(i, n)].corner == true && vi[index(i, n)].type == CPointType::CIRCLE && vi[index(i - 1, n)].type == CPointType::LINE)
			vi[index(i, n)].qie_dian = true;
		else  if (vi[index(i, n)].corner == true && vi[index(i, n)].type == CPointType::CIRCLE && vi[index(i + 1, n)].type == CPointType::LINE)
			vi[index(i, n)].qie_dian = true;
		else  if (vi[index(i, n)].corner == true && vi[index(i, n)].type == CPointType::CIRCLE && vi[index(i + 1, n)].type == CPointType::LINE)
			vi[index(i, n)].qie_dian = true;
	}
}