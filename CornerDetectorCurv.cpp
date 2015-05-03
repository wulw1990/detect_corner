#include "CornerDetectorCurv.h"

#define SQUARE(x) ((x)*(x))
#define PI 3.14159

CornerDetectorCurv::CornerDetectorCurv(double _t) :
r_(3),
thresh_curv(_t)
{
}



void CornerDetectorCurv::detect(
	std::vector<std::vector<Point> >& _vvpoint,
	std::vector<std::vector<PointInfo> >& _vvinfo)
{
	resizeToSame(_vvpoint, _vvinfo);
	for (int c = 0; c < (int)_vvpoint.size(); ++c)
		classifyEach(_vvpoint[c], _vvinfo[c]);
}

void CornerDetectorCurv::classifyEach(
	const std::vector<Point>& _vpoint,
	std::vector<PointInfo>& _vinfo)
{
	const int n_points = (int)_vpoint.size();
	if (n_points < (r_ * 2 + 1))
		return;
	std::vector<double> thetas(n_points);
	for (int ip = 0; ip < n_points; ++ip)
	{
		int ipl = ip - r_;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + r_;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		Point2d p(_vpoint[ip]);
		Point2d pl(_vpoint[ipl]);
		Point2d pr(_vpoint[ipr]);

		double h = getTriangleHeight(pl, p, pr);
		thetas[ip] = std::asin(h / r_);	
	}
	for (int ip = 0; ip < n_points; ++ip)
	{
		int ipl = ip - 1;
		ipl = (ipl < 0 ? ipl + n_points : ipl);
		int ipr = ip + 1;
		ipr = (ipr >= n_points ? ipr - n_points : ipr);

		if (thetas[ip]>thresh_curv && thetas[ip] >= thetas[ipl] && thetas[ip] >= thetas[ipr])
			_vinfo[ip].corner = true;
	}
}


double CornerDetectorCurv::getTriangleHeight(const Point2d& pl, const Point2d& p, const Point2d& pr)
{
	if (pl == pr)
		return 0;
	double di = std::abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= std::sqrt(SQUARE(pr.x - pl.x) + SQUARE(pr.y - pl.y));
	return di;
}