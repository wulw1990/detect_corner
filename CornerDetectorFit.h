#pragma once
#include "CornerDetectorBase.h"
#include "CornerDetectorFitHelper.h"

class CornerDetectorFit : public CornerDetectorBase
{
public:
	//detect
	//���ⲿ���õķ���
	//ʵ�ֻ���CornerDetectorBase�еĴ��麯��
	//����˵����CornerDetectorBase.h
	void detect(
		vector<vector<CPoint> >& vvp,
		vector<vector<CPointInfo> >& vvi);

	CornerDetectorFit(){
		line_dealer = new LineDealer();
		circle_dealer = new CircleDealer();
	}
	~CornerDetectorFit(){
		delete line_dealer;
		delete circle_dealer;
	}

private:
	//LineDealer��CircleDealer�Ķ�����CornerDetectorFitHelper��
	//���ܣ�Э�������߶κ�Բ��
	LineDealer* line_dealer;
	CircleDealer* circle_dealer;

	//detectSegment
	//���ܣ���һ�����������߶μ��
	//���룺vp -- һ��������Ӧ�����е�
	//�����vi -- ��vp��Ӧ��ÿ�������Ϣ
	void detectSegment(
		vector<CPoint>& vp,
		vector<CPointInfo>& vi);

	//detectCircleAfterSegment
	//���ܣ��ڼ�����߶κ󣬽���Բ�����
	//���룺vp -- һ��������Ӧ�����е�
	//�����vi -- ��vp��Ӧ��ÿ�������Ϣ
	void detectCircleAfterSegment(
		vector<CPoint>& vp,
		vector<CPointInfo>& vi);

	//setUnknown
	//���ܣ������󣬽��̵Ĳ�����Ϊδ֪����
	void setUnknown(vector<CPointInfo>& vi);

	//setHighCurvature
	//���ܣ�δ֪���߲��ּ��յ�
	//���룺vp -- һ��������Ӧ�����е�
	//�����vi -- ��vp��Ӧ��ÿ�������Ϣ
	//�������������룩��R -- ��������ʱʹ�õİ뾶��threshcurv -- ��ֵ
	void setHighCurvature(vector<CPoint>& vp, vector<CPointInfo>& vi, int R, double threshcurv);
};