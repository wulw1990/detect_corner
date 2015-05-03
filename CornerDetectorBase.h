#pragma once

#include <vector>
#include <string>
using namespace std;

class CornerDetectorBase
{
public:
	//定义一些所有子类共享的数据类型
	//由于需要返回给调用者，所以此处设置为public
	//不需要依赖于OpenCV
	struct Point
	{
		int x;
		int y;
		Point() : x(0), y(0){}
		Point(int _x, int _y) : x(_x), y(_y){}
		bool operator==(const Point& p)const{ return (x == p.x && y == p.y); }
		bool operator!=(const Point& p)const{ return !(x == p.x && y == p.y); }
	};
	struct Point2d
	{
		double x;
		double y;
		Point2d() : x(0), y(0){}
		Point2d(double _x, double _y) : x(_x), y(_y){}
		Point2d(CornerDetectorBase::Point p) : x(p.x), y(p.y){}
		bool operator==(const Point2d& p)const{ return (x == p.x && y == p.y); }
		bool operator!=(const Point2d& p)const{ return !(x == p.x && y == p.y); }
	};
	enum PointType{ UNKOWN, LINE, CIRCLE, FULL_CIRCLE };
	struct PointInfo
	{
		PointType type;
		int i_head;
		int len;
		bool corner;
		bool curv_corner;
		PointInfo(){ Reset(); }
		void Reset()
		{
			type = PointType::UNKOWN;
			i_head = -1;
			len = -1;
			corner=false;
			curv_corner = false;
		}
	};

	virtual void detect(
		vector<vector<Point> >& _vvpoint,
		vector<vector<PointInfo> >& _vvinfo) = 0;//纯虚函数，子类必须实现

	virtual ~CornerDetectorBase() {}

protected:
	void resizeToSame(
		const vector<vector<Point> >& _vvpoint,
		vector<vector<PointInfo> >& _vvinfo)
	{
		_vvinfo.resize(_vvpoint.size());
		for (int i = 0; i < (int)_vvpoint.size(); ++i)
			_vvinfo[i].resize(_vvpoint[i].size());
	}

	int index(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}

public:
	//依赖opencv部分
	void drawContours(
		int rows,
		int cols,
		const vector<vector<CornerDetectorBase::Point> >& _vvpoint,
		const vector<vector<CornerDetectorBase::PointInfo> >& _vvinfo,
		string save_name);
};