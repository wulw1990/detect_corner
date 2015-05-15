#include "LineDetector.h"
#include <cassert>
#include <iostream>

#define SQUARE(x) ((x)*(x))
#define PI 3.14159

LineDetector::LineDetector()
{
	length_thresh.clear();
	length_thresh.push_back(5);
	length_thresh.push_back(10);
	length_thresh.push_back(100);
	length_thresh.push_back(1000);

	error_thresh.clear();
	error_thresh.push_back(0.1);
	error_thresh.push_back(1.1);
	error_thresh.push_back(2.1);
	len_sure = 20;
}
void LineDetector::merge(vector<CPoint>& vp, vector<CPointInfo>& vi)
{
	const int N = (int)vp.size();
	for (int i = 0; i < N; ++i){
		int i_next = getIndex(i + 1, N);
		if (vi[i].corner && vi[getIndex(i+1, N)].corner
		&& (vi[i].type == CPointType::LINE) && (vi[i_next].type == CPointType::LINE)
		&& (vi[i].len >= len_sure || vi[i_next].len >= len_sure)){
			int i_head = vi[i].i_head;
			int len = vi[i].len + vi[i_next].len;
			if (isValidLongLine(vp, i_head, len))
				setInfo(vp, i_head, len, vi);
		}
	}
}
bool LineDetector::isLineSure(vector<CPointInfo>& vi, int i)
{
	const int N = (int)vi.size();
	const CPointInfo& info = vi[getIndex(i, N)];
	return (info.type == CPointType::LINE && info.len >= getLenSure());
}
bool LineDetector::isValid(vector<CPoint>& vp, int i_head, int len)
{
	const int N = (int)vp.size();
	if (len >= N) return false;//³¬¹ýÒ»È¦
	if (len < length_thresh[0]) return false;//Ì«¶Ì

	vector<double> error_vec;
	double error_thresh_max = error_thresh[error_thresh.size() - 1];
	if (!getErrorVec(vp, i_head, len, error_thresh_max, error_vec)) return false;
	double error_max = getMax(error_vec);

	if (!checkErrorToLength(len, error_max, length_thresh, error_thresh)) return false;
	if (!checkSectionError(error_vec, 0.8)) return false;
	if (!checkEndError(error_vec, 0.2)) return false;
	return true;
}
bool LineDetector::isValidLongLine(vector<CPoint>& vp, int i_head, int len)
{
	const int N = (int)vp.size();
	if (len >= N) return false;//³¬¹ýÒ»È¦
	if (len < length_thresh[0]) return false;//Ì«¶Ì

	vector<double> error_vec;
	double error_thresh_max = error_thresh[error_thresh.size() - 1];
	if (!getErrorVec(vp, i_head, len, error_thresh_max, error_vec)) return false;
	double error_max = getMax(error_vec);

	length_thresh[length_thresh.size() - 1] = (int)1e6;
	if (!checkErrorToLength(len, error_max, length_thresh, error_thresh)) return false;
	if (!checkSectionError(error_vec, 0.8)) return false;
	if (!checkEndError(error_vec, 0.2)) return false;
	return true;
}
bool LineDetector::getErrorVec(vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec)
{
	error_vec.clear();
	const int N = (int)vp.size();
	CPoint2d pl(vp[getIndex(i_head, N)]);
	CPoint2d pr(vp[getIndex(i_head+len-1, N)]);
	for (int i = 1; i < len-1; ++i)
	{
		CPoint2d p(vp[getIndex(i_head+i, N)]);
		double error = getTriangleHeight(pl, p, pr);
		error_vec.push_back(error);
		if (error > max_error) return false;
	}
	return true;
}
void LineDetector::setInfo(vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi)
{
	const int N = (int)vp.size();
	for (int i = 0; i < len; ++i)
	{
		vi[getIndex(i_head + i, N)].type = CPointType::LINE;
		vi[getIndex(i_head + i, N)].i_head = i_head;
		vi[getIndex(i_head + i, N)].len = len;
		vi[getIndex(i_head + i, N)].corner = false;
	}
	vi[getIndex(i_head, N)].corner = true;
	vi[getIndex(i_head + len - 1, N)].corner = true;
}