#include "ContourClassifierFit.h"
#include "./show/Show.h"

#define SQUARE(x) ((x)*(x))
#define PI 3.14159

/*
* Steps of algorithm:
* 1 : Detect Segment
* 2 : Detect Circle
*/
void ContourClassifierFit::Classify(
	const std::vector<std::vector<Point> >& _vvpoint,
	std::vector<std::vector<PointInfo> >& _vvinfo)
{
	ResizeToSame(_vvpoint, _vvinfo);
	for (int c = 0; c < (int)_vvpoint.size(); ++c)
	{
		//太小的轮廓不做处理
		if ((int)_vvpoint[c].size() <= 5)
			continue;
		//完整圆的检测，与其他检测完全无关，所以放在第一步
		if (circle_dealer->detectFullCircle(_vvpoint[c], _vvinfo[c]))
			continue;

		DetectSegment(_vvpoint[c], _vvinfo[c]);
		DetectCircleAfterSegment(_vvpoint[c], _vvinfo[c]);
		
		line_dealer->merge(_vvpoint[c], _vvinfo[c]);//segment larger than 1000

		line_dealer->removeShort(_vvinfo[c]);
		SetUnknown(_vvinfo[c]);

		SetHighCurvature(_vvpoint[c], _vvinfo[c], 3 , 0.38);

	}
}

//--Segment ------------------------------------------------
void ContourClassifierFit::DetectSegment(
	const std::vector<Point>& _c,
	std::vector<struct PointInfo>& _vi)
{
	const int n_points = (int)_c.size();
	for (int i = 0; i < (int)n_points;)
	{
		bool flag_success = false;
		const int len_max = line_dealer->GetLenThreshMax();
		i += line_dealer->Detect(_c, i, len_max, _vi);
	}
}

void ContourClassifierFit::DetectCircleAfterSegment(
	const std::vector<Point>& _c,
	std::vector<struct PointInfo>& _vi)
{
	const int n_points = (int)_c.size();
	for (int i_head = 0; i_head < (int)n_points; ++i_head)
	{
		if (line_dealer->isLineSure(_vi, i_head))
			continue;
		const int cir_length_max = circle_dealer->GetLenThreshMax();

		int i_back = i_head + 1;
		int max_length_small_seg = 0;
		for (int i_back_max = i_head + cir_length_max - 1; i_back <= i_back_max; ++i_back)
		{
			if (line_dealer->isLineSure(_vi, i_back))
				break;
		}
		i_back -= 1;//最后一个符合条件的点
		const int len_max = i_back - i_head + 1;
		
		int len = circle_dealer->Detect(_c, i_head, len_max, _vi);
		i_head = i_head + len - 1;
	}
}

void ContourClassifierFit::mergeLine(
	const std::vector<Point>& _c,
	std::vector<struct PointInfo>& _vi)
{
	line_dealer->merge(_c, _vi);
}

void ContourClassifierFit::SetUnknown(std::vector<struct PointInfo>& _vi)
{
	const int n_points = (int)_vi.size();
	const int min_length = 30;
	for (int i = 0; i < n_points; ++i)
	{
		if (_vi[i].type!=PointType::UNKOWN && _vi[i].len < min_length)
			ShapeDealer::resetInfo(_vi, i);
	}
		
}

double TriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr)
{
	if (pl == pr)
		return 0;
	double di = std::abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= std::sqrt(SQUARE(pr.x - pl.x) + SQUARE(pr.y - pl.y));
	return di;
}
void ContourClassifierFit::SetHighCurvature(const std::vector<Point>& vp, std::vector<PointInfo>& vi, int r_, double thresh_curv)
{
	//const int r_ = 3;
	//const double thresh_curv = 0.38f;
	const int n_points = (int)vp.size();
	if (n_points < (r_ * 2 + 1))
		return;
	std::vector<double> thetas(n_points);
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

		double h = TriangleHeight(pl, p, pr);
		thetas[ip] = std::asin(h / r_);
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