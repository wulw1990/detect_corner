#include "ShapeDetector.h"
#include <cassert>
#include <iostream>

#define SQUARE(x) ((x)*(x))
#define PI 3.14159

const double kErrorThreshShort = 0.6;
const double kErrorThreshLong = 1.0;
const double kRadiusMin = 5;
const double kRadiusMax = 100;
const double kRadianMin = 60;

int ShapeDetector::detect(vector<CPoint>& vp, int i_head, int len_max, vector<CPointInfo>& vi)
{
	//check input output
	const int N = (int)vp.size();
	if (len_max > N) len_max = N;//超过一圈

	for (int len = len_max; len >= 1; len /= 2){
		int i_back = i_head + len - 1;
		if (isValid(vp, i_head, len)){
			if (len == len_max){
				setInfo(vp, i_head, len, vi);
				return len;
			}
			else{//可能还有更长的，再仔细找
				len *= 2;
				for (;; --len){
					int i_back = i_head + len - 1;
					if (isValid(vp, i_head, len)){
						setInfo(vp, i_head, len, vi);
						return len;
					}
				}
			}
		}
	}//for
	return 1;
}
void ShapeDetector::resetInfo(vector<CPointInfo>& vi, int ip)
{
	const int N = (int)vi.size();
	int i_head = vi[getIndex(ip, N)].i_head;
	int len = vi[getIndex(ip, N)].len;
	for (int i = 0; i < len; ++i)
		vi[getIndex(i_head + i, N)].Reset();
}
bool ShapeDetector::checkSectionError(vector<double>& error_vec, double max_sec_error, int sec_len)
{
	const int vec_len = (int)error_vec.size();
	if (vec_len <= sec_len){
		double average = 0;
		for (int i = 0; i < vec_len; ++i)
			average += error_vec[i];
		average /= vec_len;
		if (average > max_sec_error) return false;
	}
	else{
		for (int i = 0; i < vec_len - sec_len; i += sec_len / 2)
		{
			double average = 0;
			for (int j = i; j < i + sec_len; ++j)
				average += error_vec[j];
			average /= sec_len;
			if (average > max_sec_error) return false;
		}
	}
	return true;
}
bool ShapeDetector::checkEndError(vector<double>& error_vec, double max_sec_error, int sec_len)
{
	const int vec_len = (int)error_vec.size();
	if (vec_len <= sec_len){
		double average = 0;
		for (int i = 0; i < vec_len; ++i)
			average += error_vec[i];
		average /= vec_len;
		if (average > max_sec_error) return false;
	}
	else{
		double average = 0;
		for (int i = 0; i <= sec_len - 1; ++i)
			average += error_vec[i];
		average /= sec_len;
		if (average > max_sec_error) return false;
		average = 0;
		for (int i = vec_len - sec_len - 1; i <= vec_len - 1; ++i)
			average += error_vec[i];
		average /= sec_len;
		if (average > max_sec_error) return false;
	}
	return true;
}
bool ShapeDetector::checkErrorToLength(
	int length,
	double error,
	vector<int>& length_thresh,
	vector<double>& error_thresh)
{
	if (length<length_thresh[0] || length>length_thresh[length_thresh.size() - 1]) return false;
	for (int i = 0; i<(int)error_thresh.size(); ++i){
		if (length >= length_thresh[i] && length <= length_thresh[i + 1]){
			if (error > error_thresh[i]) return false;
			else return true;
		}
	}
	return false;
}