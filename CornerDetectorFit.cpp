#include "CornerDetectorFit.h"

#define SQUARE(x) ((x)*(x))

void CornerDetectorFit::detect(
	vector<vector<Point> >& _vvpoint,
	vector<vector<PointInfo> >& _vvinfo)
{
	resizeToSame(_vvpoint, _vvinfo);
	for (int c = 0; c < (int)_vvpoint.size(); ++c)
	{
		if ((int)_vvpoint[c].size() <= 5) continue;
		if (circle_dealer->detectFullCircle(_vvpoint[c], _vvinfo[c])) continue;

		detectSegment(_vvpoint[c], _vvinfo[c]);
		detectCircleAfterSegment(_vvpoint[c], _vvinfo[c]);
		
		line_dealer->merge(_vvpoint[c], _vvinfo[c]);//segment larger than 1000
		line_dealer->removeShort(_vvinfo[c]);
		setUnknown(_vvinfo[c]);
		setHighCurvature(_vvpoint[c], _vvinfo[c], 3 , 0.38);
	}
}

//--Segment ------------------------------------------------
void CornerDetectorFit::detectSegment(
	vector<Point>& _c,
	vector<struct PointInfo>& _vi)
{
	int n_points = (int)_c.size();
	for (int i = 0; i < (int)n_points;)
	{
		bool flag_success = false;
		int len_max = line_dealer->getLenThreshMax();
		i += line_dealer->detect(_c, i, len_max, _vi);
	}
}

void CornerDetectorFit::detectCircleAfterSegment(
	vector<Point>& _c,
	vector<struct PointInfo>& _vi)
{
	int n_points = (int)_c.size();
	for (int i_head = 0; i_head < (int)n_points; ++i_head)
	{
		if (line_dealer->isLineSure(_vi, i_head))
			continue;
		int cir_length_max = circle_dealer->getLenThreshMax();

		int i_back = i_head + 1;
		int max_length_small_seg = 0;
		for (int i_back_max = i_head + cir_length_max - 1; i_back <= i_back_max; ++i_back)
		{
			if (line_dealer->isLineSure(_vi, i_back))
				break;
		}
		i_back -= 1;//最后一个符合条件的点
		int len_max = i_back - i_head + 1;
		
		int len = circle_dealer->detect(_c, i_head, len_max, _vi);
		i_head = i_head + len - 1;
	}
}
void CornerDetectorFit::setUnknown(vector<struct PointInfo>& _vi)
{
	int n_points = (int)_vi.size();
	int min_length = 30;
	for (int i = 0; i < n_points; ++i)
	{
		if (_vi[i].type!=PointType::UNKOWN && _vi[i].len < min_length)
			ShapeDealer::resetInfo(_vi, i);
	}
		
}
double getTriangleHeight(Point2d& pl, Point2d& p, Point2d& pr)
{
	if (pl == pr)
		return 0;
	double di = abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= sqrt(SQUARE(pr.x - pl.x) + SQUARE(pr.y - pl.y));
	return di;
}
void CornerDetectorFit::setHighCurvature(vector<Point>& vp, vector<PointInfo>& vi, int r_, double thresh_curv)
{
	//int r_ = 3;
	//double thresh_curv = 0.38f;
	int n_points = (int)vp.size();
	if (n_points < (r_ * 2 + 1))
		return;
	vector<double> thetas(n_points);
	for (int ip = 0; ip < n_points; ++ip)
	{
		if (vi[ip].type != PointType::UNKOWN)
		{
			thetas[ip] = 0;
			continue;
		}
		int ipl = ip - r_;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + r_;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		Point2d p(vp[ip]);
		Point2d pl(vp[ipl]);
		Point2d pr(vp[ipr]);

		double h = getTriangleHeight(pl, p, pr);
		thetas[ip] = asin(h / r_);
	}
	for (int ip = 0; ip < n_points; ++ip)
	{
		int ipl = ip - 1;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + 1;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		if (thetas[ip]>thresh_curv && thetas[ip] >= thetas[ipl] && thetas[ip] >= thetas[ipr])
			vi[ip].curv_corner = true;
	}
}