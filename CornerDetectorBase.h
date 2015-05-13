#pragma once

#include <vector>
#include <string>
using namespace std;

//**************************************************************
//BBNC��߳��Ե�����CornerDetecctor�̳иû���
//��Ҫ��ʵ�ֽӿڣ�detect������������һЩ�������͡���CPoint��CPointInfo
//������CornerDetecctorҪ��ֻʹ��c++��׼�⣬��Ҫ������OpenCV
//**************************************************************

class CornerDetectorBase
{
public:
	//����һЩ�������๲�����������
	//������Ҫ���ظ������ߣ����Դ˴�����Ϊpublic

	//CPoint����int�͵������
	struct CPoint
	{
		int x;
		int y;
		CPoint() : x(0), y(0){}
		CPoint(int _x, int _y) : x(_x), y(_y){}
	};

	//CPoint2d����double�͵������
	struct CPoint2d
	{
		double x;
		double y;
		CPoint2d() : x(0), y(0){}
		CPoint2d(double _x, double _y) : x(_x), y(_y){}
		CPoint2d(CornerDetectorBase::CPoint p) : x(p.x), y(p.y){}
	};

	//**************************************************************
	//enum CPointType ���� ���ܵĲ�ͬ��״
	//	UNKOWN ���� δ֪��״����Ӧ������˵�ĸ�������
	//	LINE ���� �ж�Ϊ�߶�
	//	CIRCLE ���� �ж�ΪԲ��
	//	FULL_CIRCLE ���� �ж�Ϊ����Բ
	//**************************************************************
	enum CPointType{ UNKOWN, LINE, CIRCLE, FULL_CIRCLE };

	//**************************************************************
	//CPointInfo -- ��CPoint��Ӧ��ÿ����������Ϣ
	//	type -- �õ������ε����ͣ���CPointType���壩
	//	i_head -- �õ������ε���ʵindex
	//	len -- �õ������εĳ���
	//	corner -- �õ��Ƿ�Ϊ�����εĶ˵�
	//	curv_corner -- UNKOWN�����ҵ��Ķ˵�
	//	qie_dian -- �е�
	//�����ƵĶԽӣ�������Ϊ����corner��curv_corner����Ϊtrue�ĵ�λ�յ�
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
	//���麯�����������ʵ��
	//���ܣ����йյ���
	//���룺
	//	vvp������ǰͼ������������
	//	vector<CPoint>��ʾһ������
	//	vector<vector<CPoint> >��ʾ�������
	//�����
	//	vvi����ÿ�������Ϣ��
	//	��������һһ��Ӧ��ϵ
	//**************************************************************
	virtual void detect(
		vector<vector<CPoint> >& vvp,
		vector<vector<CPointInfo> >& vvi) = 0;

	virtual ~CornerDetectorBase() {}

protected:
	//resizeToSame
	//���ܣ���vvi����resize����vvpһһ��Ӧ
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
	//���ܣ�����һ�����ʵ���������е�λ��
	//���磺ĳ����������100����
	//	���0����洢�������0λ
	//	���99����洢�������99λ
	//	���100����洢�������0λ��ѭ��������
	//���룺i�����ڼ�����
	//�����n������������һ���м�����
	//**************************************************************
	int getIndex(int i, int n)
	{
		i %= n;
		if (i < 0)
			i += n;
		return i;
	}
};
//Note:
//wrong : for (int i = i_head; i <= i_back; ++i) wrong
//right��Note:for (int i = 0; i < len; ++i)��
//getIndex(i_head+i, N)