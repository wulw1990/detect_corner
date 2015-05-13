#pragma once
#include "ShapeDetector.h"

class LineDetector : public ShapeDetector
{
public:
	LineDetector();
	int getLenSure(){ return len_sure; }
	void merge(vector<CPoint>& vp, vector<CPointInfo>& vi);
	bool isLineSure(vector<CPointInfo>& vi, int i);
	int getLenThreshMax(){ return length_thresh[length_thresh.size() - 1]; }
private:
	int len_sure;

	vector<int> length_thresh;
	vector<double> error_thresh;

	bool isValid(vector<CPoint>& vp, int i_head, int len);
	void setInfo(vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi);
	bool getErrorVec(vector<CPoint>& vp, int i_head, int len, double max_error, vector<double>& error_vec);

	bool isValidLongLine(vector<CPoint>& vp, int i_head, int len);
};
