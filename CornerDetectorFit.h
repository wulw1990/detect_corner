#pragma once
#include "CornerDetectorBase.h"
#include "CircleDetector.h"
#include "LineDetector.h"

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

	//setThresh
	//���ܣ��Զ�����ֵ
	//���룺
	//	circle_error_thresh_short -- ��Բ���������С��20�����أ�
	//	circle_error_thresh_long -- ��Բ����������
	//	circle_radius_min -- Բ������С�뾶
	//	circle_radius_max -- Բ�������뾶
	//	circle_radian_min -- Բ������С���ȣ���λ�Ƕ���Ĭ����60�ȣ�
	void setThresh(
		double circle_error_thresh_short = 0.6,
		double circle_error_thresh_long = 1.0,
		double circle_radius_min = 5,
		double circle_radius_max = 100,
		double circle_radian_min = 60)
	{
		circle_dealer->setThresh(
			circle_error_thresh_short,
			circle_error_thresh_long,
			circle_radius_min,
			circle_radius_max,
			circle_radian_min);
	}

	CornerDetectorFit(){
		line_dealer = new LineDetector();
		circle_dealer = new CircleDetector();
	}
	~CornerDetectorFit(){
		delete line_dealer;
		delete circle_dealer;
	}

private:
	//LineDealer��CircleDealer�Ķ�����CornerDetectorFitHelper��
	//���ܣ�Э�������߶κ�Բ��
	LineDetector* line_dealer;
	CircleDetector* circle_dealer;

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

	//setQieDian
	//���ܣ������е�
	//���룺vi -- ��vp��Ӧ��ÿ�������Ϣ
	//�����vi -- ��vp��Ӧ��ÿ�������Ϣ
	void setQieDian(vector<CPointInfo>& vi);
}; 