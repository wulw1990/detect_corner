#include "CornerDetectorFitHelper.h"
#include <cassert>
#include <iostream>

#define SQUARE(x) ((x)*(x))
#define PI 3.14159

//-------------------------------------------------------------------------------------------------LineDealer---------
int ShapeDealer::detect(
	const std::vector<CPoint>& vp, int i_head, int len_max,
	std::vector<CPointInfo>& vi)
{
	//check input output
	const int n_points = (int)vp.size();
	if (len_max > n_points)//超过一圈
		len_max = n_points;

	for (int len = len_max; len >= 1; len /= 2)//二分法查找
	{
		int i_back = i_head + len - 1;
		if (isValid(vp, i_head, len))
		{
			if (len == len_max)
			{
				setInfo(vp, i_head, len, vi);
				return len;
			}
			else//可能还有更长的，在仔细找一次
			{
				len *= 2;
				for (;; --len)
				{
					int i_back = i_head + len - 1;
					if (isValid(vp, i_head, len))
					{
						setInfo(vp, i_head, len, vi);
						return len;
					}
				}
			}
		}
		
	}
	//没有valid
	return 1;
}
void ShapeDealer::resetInfo(std::vector<CPointInfo>& vi, int ip)
{
	const int n_points = (int)vi.size();
	ip = index(ip, n_points);
	int i_head = vi[ip].i_head;
	int len = vi[ip].len;
	int i_back = i_head + len - 1;
	for (int i = i_head; i <= i_back; ++i)
		vi[index(i, n_points)].Reset();
}
double ShapeDealer::getTriangleHeight(const CPoint2d& pl, const CPoint2d& p, const CPoint2d& pr)
{
	assert(pl != pr);
	double di = std::abs((pr.y - pl.y)*p.x - (pr.x - pl.x)*p.y + (pr.x*pl.y - pl.x*pr.y));
	di /= std::sqrt(SQUARE(pr.x - pl.x) + SQUARE(pr.y - pl.y));
	return di;
}
double ShapeDealer::vecMax(std::vector<double> vec)
{
	double max = -1e6;
	for (int i = 0; i < (int)vec.size(); ++i)
		if (vec[i]>max)
			max = vec[i];
	return max;
}
double ShapeDealer::secAverageMax(std::vector<double> vec, int sec_len)
{
	const int vec_len = (int)vec.size();
	if (vec_len <= sec_len)
	{
		double average = 0;
		for (int i = 0; i < vec_len; ++i)
			average += vec[i];
		average /= vec_len;
		return average;
	}

	double max = 1e-10;
	for (int i = 0; i < vec_len - sec_len; ++i)
	{
		double average = 0;
		for (int sub_i = i; sub_i < i + sec_len; ++sub_i)
			average += vec[sub_i];
		average /= sec_len;
		if (average > max)
			max = average;
	}
	return max;
}
double ShapeDealer::secAverageMaxEnd(std::vector<double> vec, int sec_len)
{
	const int vec_len = (int)vec.size();
	if (vec_len <= sec_len)
	{
		double average = 0;
		for (int i = 0; i < vec_len; ++i)
			average += vec[i];
		average /= vec_len;
		return average;
	}
	double max = 1e-10;
	{
		double average = 0;
		for (int i = 0; i <= sec_len - 1; ++i)
			average += vec[i];
		average /= sec_len;
		if (average > max)
			max = average;
	}
	{
		double average = 0;
		for (int i = vec_len - sec_len - 1; i <= vec_len - 1; ++i)
			average += vec[i];
		average /= sec_len;
		if (average > max)
			max = average;
	}
	return max;
}
bool ShapeDealer::secCheck(
	int length,
	double error,
	const std::vector<int>& length_thresh,
	const std::vector<double>& error_thresh)
{
	if (length<length_thresh[0] || length>length_thresh[length_thresh.size() - 1])
		return false;

	for (int is = 0; is<(int)error_thresh.size(); ++is)
	{
		if (length >= length_thresh[is] && length <= length_thresh[is + 1])//length总会落在某个区间
		{
			if (error > error_thresh[is])
				return false;
			else
				return true;
		}
	}
	return true;
}

//-------------------------------------------------------------------------------------------------LineDealer---------
LineDealer::LineDealer()
{
	length_thresh = { 5, 10, 100, 1000 };
	error_thresh = { 0.1, 1.1, 2.1 };
	len_sure = 20;
}
void LineDealer::merge(const std::vector<CPoint>& vp, std::vector<CPointInfo>& vi)
{
	const double thresh = 1.1;
	const int n_points = (int)vp.size();
	for (int ip = 0; ip < n_points; ++ip)
	{
		if (vi[ip].corner && vi[(ip + 1) % n_points].corner
			&& (vi[ip].type == CPointType::LINE) && (vi[(ip + 1) % n_points].type == CPointType::LINE)
			&& (vi[ip].len >= len_sure || vi[(ip + 1) % n_points].len>=len_sure))
		{
			int ipl = vi[ip].i_head;
			int ipr = vi[index(ip + 1, n_points)].i_head + vi[index(ip + 1, n_points)].len - 1;
			if (ipr < ipl)
				ipr += n_points;

			CPoint2d pl(vp[index(ipl, n_points)]);
			CPoint2d pr(vp[index(ipr, n_points)]);
			bool can_merge = true;
			for (int im = ipl + 1; im < ipr; ++im)
			{
				CPoint2d pm(vp[index(im, n_points)]);
				if (getTriangleHeight(pl, pm, pr) > thresh)
				{
					can_merge = false;
					break;
				}
				//std::cout << "\t" << im ;
				//std::cout << "\tn_points : " << n_points << std::endl;
			}
			if (can_merge)
			{
				int i_head_new = vi[ip].i_head;
				int len_new = vi[ip].len + vi[(ip + 1) % n_points].len;
				resetInfo(vi, ip);
				resetInfo(vi, ip+1);
				setInfo(vp, i_head_new, len_new, vi);
			}
			//std::cout << "\t" << ip << std::endl;
		}
	}
}
bool LineDealer::isLineSure(const std::vector<CPointInfo>& vi, int i)
{
	const int n_points = (int)vi.size();
	const CPointInfo& info = vi[index(i, n_points)];
	return (info.type == CPointType::LINE && info.len >= getLenSure());
}
bool LineDealer::isValid(const std::vector<CPoint>& vp, int i_head, int len)
{
	//check input, output
	const int n_points = (int)vp.size();
	if (len > n_points)//超过一圈
		return false;
	if (len < length_thresh[0])//太短
		return false;

	//calculate seg average error
	const int sec_len = 10;
	vector<double> error_vec;
	if (!getErrorVec(vp, i_head, len, error_thresh[error_thresh.size() - 1], error_vec))
		return false;//尽早退出
	double error_max = vecMax(error_vec);
	double error_sec_aver_max = secAverageMax(error_vec, sec_len);
	double error_sec_aver_max_end = secAverageMaxEnd(error_vec, sec_len);

	//judge the error_max, for each length type
	if (!secCheck(len, error_max, length_thresh, error_thresh))
		return false;

	//judge the error_section_average_max
	if (error_sec_aver_max > 0.8)
		return false;
	if (error_sec_aver_max_end > 0.2)
		return false;

	//TODO : delete ? 
	//head and tail
	if (len >= sec_len)
	{
		bool head_tail_flag = false;
		for (int i = len - sec_len - 1; i >= len - 1; ++i)
			if (error_vec[i] >= error_vec[len - 1] - 1e-6)
				head_tail_flag = true;
		for (int i = 0; i <= sec_len; ++i)
			if (error_vec[i] >= error_vec[0] - 1e-6)
				head_tail_flag = true;
		if (!head_tail_flag)
			return false;
	}
	//OK
	return true;
}
bool LineDealer::getErrorVec(const std::vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec)
{
	error_vec.clear();
	const int n_points = (int)vp.size();
	int i_back = i_head + len - 1;
	CPoint2d pl(vp[i_head%n_points]);
	CPoint2d pr(vp[i_back%n_points]);
	for (int index = i_head + 1; index < i_back; ++index)
	{
		CPoint2d p(vp[index%n_points]);
		double error = getTriangleHeight(pl, p, pr);
		error_vec.push_back(error);
		if (error > max_error)
			return false;
	}
	return true;
}
void LineDealer::setInfo(const std::vector<CPoint>& vp, int i_head, int len, std::vector<CPointInfo>& vi)
{
	const int n_points = (int)vp.size();
	int i_back = i_head + len - 1;
	for (int i = i_head; i <= i_back; ++i)
	{
		if (vi[index(i, n_points)].type == CPointType::LINE)
		{
			/*resetInfo(vi, i);
			break;*/
			int i_head_old = vi[index(i, n_points)].i_head;
			int len_old = vi[index(i, n_points)].len;
			int i_back_old = i_head_old + len_old - 1;
			i_head_old += n_points;//TODO 有点绕 多绕一圈
			i_back_old += n_points;

			if (i_back_old > i_back%n_points)
			{
				int len_total = i_back_old - i_head + 1;
				if (isValid(vp, i_head, len_total))
				{
					i_back = i_back_old;
					len = i_back - i_head + 1;
				}
				else
				{
					for (int i = i_back + 1; i <= i_back_old; ++i)
					{
						vi[index(i, n_points)].type = CPointType::LINE;
						vi[index(i, n_points)].i_head = i_back + 1;
						vi[index(i, n_points)].len = i_back_old - i_back;
						//std::cout << "i_back_old - i_back : " << i_back_old - i_back << std::endl;
						vi[index(i, n_points)].corner = false;
					}
					vi[index(i_back + 1, n_points)].corner = true;
					vi[index(i_back_old, n_points)].corner = true;
				}
			}
		}
	}
	for (int i = i_head; i <= i_back; ++i)
	{
		vi[index(i, n_points)].type = CPointType::LINE;
		vi[index(i, n_points)].i_head = i_head;
		vi[index(i, n_points)].len = len;
		vi[index(i, n_points)].corner = false;
	}
	vi[i_head % n_points].corner = true;
	vi[i_back % n_points].corner = true;
}
void LineDealer::setUnknown(vector<CPointInfo>& vi)
{
	int n_points = (int)vi.size();
	int min_length = 30;
	for (int i = 0; i < n_points; ++i)
	{
		if (vi[i].type != CPointType::UNKOWN && vi[i].len < min_length)
			ShapeDealer::resetInfo(vi, i);
	}
}
//-------------------------------------------------------------------------------------------------CircleDealer---------
CircleDealer::CircleDealer()
{
	length_thresh = { 10, 20, 100, 1000 };
	error_thresh = { 0.6, 1.0, 1.0 };
	//cir_error_aver_vec_ = { 1.1, 2.1, 3.1 };
	radius_min = 5;
	radius_max = 100;
	radian_min = 60 / 180.0 * PI;
	center_ = CPoint2d(-1, -1);
	radius_ = -1;
}
bool CircleDealer::detectFullCircle(const std::vector<CPoint>& vp, std::vector<CPointInfo>& vi)
{
	const int n_points = (int)vp.size();
	assert(n_points >= 3);//线性拟合的前提条件
	CPoint2d center;
	double radius;
	fitCircleLinear(vp, center, radius);
	//printf("center : (%f, %f), radius : %f\n", center.x, center.y, radius);
	if (!(radius > 0))
		return false;

	const double thresh = 1.0;//TODO : best thresh ?
	bool is_full_circle = true;
	for (int i = 0; i < n_points; ++i)//大部分轮廓不用计算所有误差（break）
	{
		CPoint2d p(vp[i]);
		if (getError(p, center, radius) > thresh)
		{
			is_full_circle = false;
			break;//尽早退出
		}
	}
	if (is_full_circle)
	{
		for (int i = 0; i < n_points; ++i)
		{
			vi[index(i, n_points)].type = CPointType::FULL_CIRCLE;
			vi[index(i, n_points)].i_head = 0;
			vi[index(i, n_points)].len = n_points;
			vi[index(i, n_points)].corner = false;
		}
		return true;
	}
	else
		return false;
}
void CircleDealer::merge(const std::vector<CPoint>& vp, std::vector<CPointInfo>& vi)
{
	const int n_points = (int)vp.size();
	for (int ip = 0; ip < n_points; ++ip)
	{
		if (vi[ip].corner && vi[(ip + 1) % n_points].corner)
		{
			int i_head_new = vi[ip].i_head;
			int len_new = vi[ip].len + vi[(ip + 1) % n_points].len;
			getCircle(vp, i_head_new, len_new, center_, radius_);

			CPoint2d p(vp[ip]);
			double error = (center_.x - p.x)*(center_.x - p.x) + (center_.y - p.y)*(center_.y - p.y);
			if (abs(error) < 1e-8)
				error = 1e-8;
			error = std::abs(std::sqrt(error) - radius_);
			if (error <= 2.0)
				setInfo(vp, i_head_new, len_new, vi);
		}
	}
}
bool CircleDealer::isValid(const std::vector<CPoint>& vp, int i_head, int len)
{
	//用到成员变量center_， radius_
	//check input, output
	const int n_points = (int)vp.size();
	if (len < length_thresh[0])
		return false;
	if (len > n_points)//超过一圈
		return false;

	//getCircleLinear(vp, i_head, len, center_, radius_);
	getCircle(vp, i_head, len, center_, radius_);
	
	//std::cout << "circle : " << center_.x << std::endl;
	if (!(radius_ > 0))//note : not if(radius<=0), -NAN
		return false;

	//check radius
	if (radius_ < radius_min || radius_>radius_max)
		return false;

	//check radian
	double dd = 0;
	dd = SQUARE(double(vp[index(i_head, n_points)].x) - double(vp[index(i_head + len - 1, n_points)].x));
	dd += SQUARE(double(vp[index(i_head, n_points)].y) - double(vp[index(i_head + len - 1, n_points)].y));
	if (dd < 1e-8)
		dd = 1e-8;
	dd = std::sqrt(dd);
	dd /= 2;
	double radian = std::asin(dd / radius_) * 2;
	double radian1 = (double)len / radius_;
	//std::cout << "radian : " << radian << std::endl;
	//std::cout << "radian1 : " << radian1 << std::endl;
	if (radian < radian_min)
		return false;

	//check error
	//calculate error
	const int sec_len = 10;
	//std::cout << "sec_len : " << sec_len << std::endl;
	std::vector<double> error_vec;
	if (!getErrorVec(vp, i_head, len, error_thresh[error_thresh.size() - 1], error_vec))
		return false;//尽早退出
	//std::cout << "error_vec : " << error_vec.size() << std::endl;

	double error_max = vecMax(error_vec);
	double error_sec_aver_max = secAverageMax(error_vec, sec_len);
	double error_sec_aver_max_be = secAverageMaxEnd(error_vec, sec_len);

	//judge the error_max, for each length type
	if (!secCheck(len, error_max, length_thresh, error_thresh))
		return false;

	if (error_sec_aver_max > 0.8)
		return false;
	if (error_sec_aver_max_be > 0.5)
		return false;

	return true;
	
}
bool CircleDealer::getErrorVec(const std::vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec
	, CPoint2d& center, double& radius)
{
	error_vec.clear();
	const int n_points = (int)vp.size();
	const int i_back = i_head + len - 1;
	for (int index = i_head; index <= i_back; ++index)
	{
		CPoint2d p(vp[index%n_points]);
		double tmp = (center.x - p.x)*(center.x - p.x) + (center.y - p.y)*(center.y - p.y);
		if (abs(tmp) < 1e-8)
			tmp = 1e-8;
		tmp = std::abs(std::sqrt(tmp) - radius);
		error_vec.push_back(tmp);
		if (tmp > max_error)
			return false;
	}
	return true;
}
bool CircleDealer::getErrorVec(const vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec)
{
	//用到成员变量center_， radius_
	return getErrorVec(vp, i_head, len, max_error, error_vec, center_, radius_);
}
void CircleDealer::setInfo(const std::vector<CPoint>& vp, int i_head, int len, std::vector<CPointInfo>& vi)
{
	const int n_points = (int)vp.size();
	int i_back = i_head + len - 1;
	for (int i = i_head; i <= i_back; ++i)
		if (vi[index(i, n_points)].type != CPointType::CIRCLE && vi[index(i, n_points)].type != CPointType::UNKOWN)
			resetInfo(vi, i);
	for (int i = i_head; i <= i_back; ++i)
	{
		if (vi[index(i, n_points)].type == CPointType::CIRCLE)
		{
			int i_head_old = vi[index(i, n_points)].i_head ;
			int len_old = vi[index(i, n_points)].len;
			int i_back_old = i_head_old + len_old - 1;
			i_head_old += n_points;//TODO 有点绕 多绕一圈
			i_back_old += n_points;

			if (i_back_old > i_back%n_points)
			{
				int len_total = i_back_old - i_head + 1;
				if (isValid(vp, i_head, len_total))
				{
					i_back = i_back_old;
					len = i_back - i_head + 1;
				}
				else
				{
					for (int i = i_back + 1; i <= i_back_old; ++i)
					{
						vi[index(i, n_points)].type = CPointType::CIRCLE;
						vi[index(i, n_points)].i_head = i_back + 1;
						vi[index(i, n_points)].len = i_back_old - i_back;
						//std::cout << "i_back_old - i_back : " << i_back_old - i_back << std::endl;
						vi[index(i, n_points)].corner = false;
					}
					vi[index(i_back + 1, n_points)].corner = true;
					vi[index(i_back_old, n_points)].corner = true;
				}
			}
		}
	}
	for (int i = i_head; i <= i_back; ++i)
	{
		vi[index(i, n_points)].type = CPointType::CIRCLE;
		vi[index(i, n_points)].i_head = i_head;
		vi[index(i, n_points)].len = len;
		vi[index(i, n_points)].corner = false;
	}
	vi[i_head % n_points].corner = true;
	vi[i_back % n_points].corner = true;
}
void CircleDealer::setUnknown(vector<CPointInfo>& vi)
{
	int n_points = (int)vi.size();
	int min_length = 30;
	for (int i = 0; i < n_points; ++i)
	{
		if (vi[i].type != CPointType::UNKOWN && vi[i].len < min_length)
			ShapeDealer::resetInfo(vi, i);
	}
}
void CircleDealer::getCircle(
	const std::vector<CPoint>& vp, int i_head, int len, CPoint2d& center, double& radius)
{
	//check input output
	const int n_points = (int)vp.size();
	const int i_back = i_head + len - 1;
	int left = i_head%n_points;
	int mid = ((i_head + i_back) / 2) % n_points;
	int right = i_back%n_points;

	center = CPoint2d(-1, -1);
	radius = -1;
	if (left == mid || mid == right || right == left)
		return;

	//TODO : more accurate, RANSAC ? 
	//calculate
	CPoint2d p1(vp[left]);
	CPoint2d p2(vp[mid]);
	CPoint2d p3(vp[right]);
	getCircle(p1, p2, p3, center, radius);
}
void CircleDealer::getCircle(
	const CPoint2d& p1, const CPoint2d& p2, const CPoint2d& p3,
	CPoint2d& _center, double& _radius)
{
	double x1 = p1.x;
	double x2 = p2.x;
	double x3 = p3.x;

	double y1 = p1.y;
	double y2 = p2.y;
	double y3 = p3.y;

	// PLEASE CHECK FOR TYPOS IN THE FORMULA :)
	_center.x = (x1*x1 + y1*y1)*(y2 - y3) + (x2*x2 + y2*y2)*(y3 - y1) + (x3*x3 + y3*y3)*(y1 - y2);
	_center.x /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

	_center.y = (x1*x1 + y1*y1)*(x3 - x2) + (x2*x2 + y2*y2)*(x1 - x3) + (x3*x3 + y3*y3)*(x2 - x1);
	_center.y /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

	_radius = std::sqrt((_center.x - x1)*(_center.x - x1) + (_center.y - y1)*(_center.y - y1));
}
void CircleDealer::getCircleLinear(
	const std::vector<CPoint>& vp, int i_head, int len, CPoint2d& center, double& radius)
{
	std::vector<CPoint> points;
	for (int i = i_head, i_back = i_head + len - 1; i <= i_back; ++i)
		points.push_back(vp[i]);
	fitCircleLinear(points, center, radius);
}

void CircleDealer::completeCircle(
	const std::vector<CPoint>& _c,
	std::vector<CPointInfo>& _vi)
{
	const int n_points = (int)_vi.size();

	//如果一个轮廓只有少部分点不是属于Circle，则说明该轮廓是一个完整的圆
	int count_other_point = 0;
	for (int ip = 0; ip < n_points; ++ip)
		if (_vi[ip].type != CPointType::CIRCLE)
			++count_other_point;
	if (count_other_point <= 10)//TODO 3 or 10 ?
	{
		bool is_full_circle = false;
		if (n_points < 20)//很小的完整圆
			is_full_circle = true;
		else if (isValid(_c, n_points / 2 + 5, n_points-10))
			is_full_circle = true;
		else if (isValid(_c, n_points / 2 + 9, n_points - 18))
			is_full_circle = true;

		if (is_full_circle)
		{
			for (int ip = 0; ip < n_points; ++ip)
			{
				_vi[ip].corner = false;
				_vi[ip].i_head = 0;
				_vi[ip].len = n_points;
				_vi[ip].type = CPointType::CIRCLE;
			}
		}
	}

	if (n_points < 20)	return;

	//处理刚开始被判断为线段的一部分
	{
		int circle_length_max = 0;
		for (int p = 0; p <n_points; ++p)
		{
			if (_vi[p].type == CPointType::CIRCLE && _vi[p].len > circle_length_max)
				circle_length_max = _vi[p].len;

		}
		if (n_points - circle_length_max < 30 && (double)circle_length_max / n_points >= 0.9)
		{
			for (int ip = 0; ip < n_points; ++ip)
			{
				_vi[ip].corner = false;
				_vi[ip].i_head = 0;
				_vi[ip].len = n_points;
				_vi[ip].type = CPointType::CIRCLE;
			}
		}
	}


}

//TODO : 这里使用了opencv，之后可以使用主要是因为计算b = (A.t()*A).inv()*A.t()*Y;
#include "opencv2/opencv.hpp"
void CircleDealer::fitCircleLinear(std::vector<CPoint> points, CPoint2d& center, double& radius)
{
	const int n = (int)points.size();
	assert(n >= 3);
	cv::Mat A(n, 3, CV_64FC1);
	cv::Mat b(3, 1, CV_64FC1);
	cv::Mat Y(n, 1, CV_64FC1);

	for (int i = 0; i < n; ++i)
	{
		A.at<double>(i, 0) = 1;
		A.at<double>(i, 1) = points[i].y;
		A.at<double>(i, 2) = points[i].x;

		Y.at<double>(i, 0) = -points[i].x*points[i].x - points[i].y*points[i].y;
	}

	b = (A.t()*A).inv()*A.t()*Y;
	//std::cout << b << std::endl;
	double AA = b.at<double>(2, 0);
	double BB = b.at<double>(1, 0);
	double CC = b.at<double>(0, 0);

	center.x = AA / (-2.0);
	center.y = BB / (-2.0);
	radius = std::sqrt(center.x*center.x + center.y*center.y - CC);
}
