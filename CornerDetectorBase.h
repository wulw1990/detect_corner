#pragma once

#include <vector>
#include <string>
using namespace std;

//**************************************************************
//BBNC这边尝试的所有CornerDetecctor继承该基类
//主要是实现接口：detect函数，并共享一些数据类型——CPoint，CPointInfo
//对所有CornerDetecctor要求：只使用c++标准库，不要依赖于OpenCV
//**************************************************************

class CornerDetectorBase
{
public:
	//定义一些所有子类共享的数据类型
	//由于需要返回给调用者，所以此处设置为public

	//CPoint——int型的坐标点
	struct CPoint
	{
		int x;
		int y;
		CPoint() : x(0), y(0){}
		CPoint(int _x, int _y) : x(_x), y(_y){}
		bool operator==(const CPoint& p)const{ return (x == p.x && y == p.y); }
		bool operator!=(const CPoint& p)const{ return !(x == p.x && y == p.y); }
	};

	//CPoint2d——double型的坐标点
	struct CPoint2d
	{
		double x;
		double y;
		CPoint2d() : x(0), y(0){}
		CPoint2d(double _x, double _y) : x(_x), y(_y){}
		CPoint2d(CornerDetectorBase::CPoint p) : x(p.x), y(p.y){}
		bool operator==(const CPoint2d& p)const{ return (x == p.x && y == p.y); }
		bool operator!=(const CPoint2d& p)const{ return !(x == p.x && y == p.y); }
	};

	//**************************************************************
	//enum CPointType —— 可能的不同形状
	//	UNKOWN —— 未知形状，对应凌云所说的复杂曲线
	//	LINE —— 判断为线段
	//	CIRCLE —— 判断为圆弧
	//	FULL_CIRCLE —— 判断为完整圆
	//**************************************************************
	enum CPointType{ UNKOWN, LINE, CIRCLE, FULL_CIRCLE };

	//**************************************************************
	//CPointInfo -- 与CPoint对应的每个点的输出信息
	//	type -- 该点所属段的类型（将CPointType定义）
	//	i_head -- 该点所属段的其实index
	//	len -- 该点所属段的长度
	//	corner -- 该点是否为所属段的端点
	//	curv_corner -- UNKOWN部分找到的端点
	//	qie_dian -- 切点
	//与凌云的对接：可以认为所有corner和curv_corner被置为true的点位拐点
	//**************************************************************
	struct CPointInfo
	{
		CPointType type;
		int i_head;
		int len;
		bool corner;
		bool curv_corner;
		bool qie_dian;
		CPointInfo(){ Reset(); }
		void Reset()
		{
			type = CPointType::UNKOWN;
			i_head = -1;
			len = -1;
			corner=false;
			curv_corner = false;
			qie_dian = false;
		}
	};

	//**************************************************************
	//detect
	//纯虚函数，子类必须实现
	//功能：进行拐点检测
	//输入：
	//	vvp——当前图像所有轮廓，
	//	vector<CPoint>表示一个轮廓
	//	vector<vector<CPoint> >表示多个轮廓
	//输出：
	//	vvi——每个点的信息，
	//	与输入是一一对应关系
	//**************************************************************
	virtual void detect(
		vector<vector<CPoint> >& vvp,
		vector<vector<CPointInfo> >& vvi) = 0;

	virtual ~CornerDetectorBase() {}

protected:
	//resizeToSame
	//功能：将vvi进行resize到和vvp一一对应
	void resizeToSame(
		const vector<vector<CPoint> >& vvp,
		vector<vector<CPointInfo> >& vvi)
	{
		vvi.resize(vvp.size());
		for (int i = 0; i < (int)vvp.size(); ++i)
			vvi[i].resize(vvp[i].size());
	}

	//**************************************************************
	//index
	//功能：计算一个点的实际在数组中的位置
	//例如：某个轮廓共计100个点
	//	则第0个点存储在数组的0位
	//	则第99个点存储在数组的99位
	//	则第100个点存储在数组的0位（循环往复）
	//输入：i——第几个点
	//输出：n——所在轮廓一共有几个点
	//**************************************************************
	int index(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
};