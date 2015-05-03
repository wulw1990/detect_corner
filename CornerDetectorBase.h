#pragma once

#include <vector>
#include <string>
using namespace std;

class CornerDetectorBase
{
public:
	//����һЩ�������๲�����������
	//������Ҫ���ظ������ߣ����Դ˴�����Ϊpublic
	//����Ҫ������OpenCV
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
		vector<vector<PointInfo> >& _vvinfo) = 0;//���麯�����������ʵ��

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
	//����opencv����
	void drawContours(
		int rows,
		int cols,
		const vector<vector<CornerDetectorBase::Point> >& _vvpoint,
		const vector<vector<CornerDetectorBase::PointInfo> >& _vvinfo,
		string save_name);
};