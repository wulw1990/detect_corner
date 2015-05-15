#include "CircleDetector.h"
#include <cassert>
#include <iostream>

static const double PI = 3.14159265354;

CircleDetector::CircleDetector()
{
	length_thresh.clear();
	length_thresh.push_back(10);
	length_thresh.push_back(20);
	length_thresh.push_back(1000);

	error_thresh.clear();
	error_thresh.push_back(0.6);
	error_thresh.push_back(1.0);

	radius_min = 5;
	radius_max = 100;
	radian_min = 60 / 180.0 * PI;
}
bool CircleDetector::detectFullCircle(vector<CPoint>& vp, vector<CPointInfo>& vi)
{
	const int N = (int)vp.size();
	assert(N >= 3);//线性拟合的前提条件
	CircleInfo circle = getCircleLinear(vp);
	if (!(circle.radius > 0)) return false;

	const double thresh = 1.0;
	for (int i = 0; i < N; ++i)
		if (getCircleError(CPoint2d(vp[i]), circle) > thresh) return false;

	for (int i = 0; i < N; ++i)
	{
		vi[i].type = CPointType::FULL_CIRCLE;
		vi[i].i_head = 0;
		vi[i].len = N;
		vi[i].corner = false;
	}
	return true;
}
bool CircleDetector::isValid(vector<CPoint>& vp, int i_head, int len)
{
	const int N = (int)vp.size();
	if (len < length_thresh[0]) return false;
	if (len >= N) return false;//超过一圈

	vector<CircleInfo> circle_vec;
	if (!getCircleMulti(vp, i_head, len, circle_vec)) return false;
	CircleInfo circle = circle_vec[0];

	for (int i = 0; i < (int)circle_vec.size(); ++i)
		if (checkCircle(vp, i_head, len, circle_vec[i])) return true;

	return false;//没有一个圆满足条件
}
bool CircleDetector::checkCircle(vector<CPoint>& vp, int i_head, int len, CircleInfo circle)
{
	if (!checkCircleRadius(circle.radius)) return false;
	if (!checkCircleRadian(vp, i_head, len, circle)) return false;
	if (!checkCircleError(vp, i_head, len, circle))  return false;
	return true;
}
bool CircleDetector::checkCircleRadius(double radius)
{
	if (!(radius > 0)) return false;//note : not if(radius<=0), -NAN
	if (radius < radius_min || radius > radius_max) return false;
	return true;
}
bool CircleDetector::checkCircleRadian(vector<CPoint>& vp, int i_head, int len, CircleInfo circle)
{
	const int N = (int)vp.size();
	double dd = getDistance(vp[getIndex(i_head, N)], vp[getIndex(i_head + len - 1, N)]);
	double radian = asin(dd / 2 / circle.radius) * 2;
	if (radian < radian_min) return false;
	return true;
}
bool CircleDetector::checkCircleError(vector<CPoint>& vp, int i_head, int len, CircleInfo circle)
{
	const int sec_len = 10;
	vector<double> error_vec;
	if (!getCircleErrorVec(vp, i_head, len, error_thresh[error_thresh.size() - 1], error_vec, circle)) return false;
	double error_max = getMax(error_vec);

	//judge the error_max, for each length type
	if (!checkErrorToLength(len, error_max, length_thresh, error_thresh)) return false;
	if (!checkSectionError(error_vec, 0.8)) return false;
	if (!checkEndError(error_vec, 0.4)) return false;

	return true;
}
bool CircleDetector::getCircleErrorVec(
	std::vector<CPoint>& vp, int i_head, int len,
	double max_error,
	vector<double>& error_vec,
	CircleInfo circle)
{
	error_vec.clear();
	const int N = (int)vp.size();
	for (int i = 0; i < len; ++i)
	{
		CPoint2d p(vp[getIndex(i_head + i ,N)]);
		double error = getCircleError(p, circle);
		error_vec.push_back(error);
		if (error > max_error) return false;
	}
	return true;
}
bool CircleDetector::getCircleMulti(vector<CPoint>& vp, int i_head, int len, vector<CircleInfo>& circle_vec)
{
	circle_vec.clear();

	const int N = (int)vp.size();

	CPoint2d p1(vp[getIndex(i_head, N)]);
	CPoint2d p2(vp[getIndex(i_head + len / 2, N)]);
	CPoint2d p3(vp[getIndex(i_head + len - 1, N)]);
	CircleInfo circle = getCircle(p1, p2, p3);
	
	for (int i = 0; i < len; ++i)
		if (getCircleError(CPoint2d(vp[getIndex(i_head + i, N)]), circle) > 2.0) return false;//尽早退出

	circle_vec.push_back(circle);

	//for (int x1 = -1; x1 <= 1; x1+=2)
	//	for (int x2 = -1; x2 <= 1; x2+=2)
	//		for (int x3 = -1; x3 <= 1; x3+=2)
	//			for (int y1 = -1; y1 <= 1; y1+=2)
	//				for (int y2 = -1; y2 <= 1; y2+=2)
	//					for (int y3 = -1; y3 <= 1; y3+=2)
	//					{
	//	CPoint2d p1new = CPoint2d(p1.x + 0.5*x1, p1.y + 0.5 * y1);
	//	CPoint2d p2new = CPoint2d(p2.x + 0.5*x2, p2.y + 0.5 * y2);
	//	CPoint2d p3new = CPoint2d(p3.x + 0.5*x3, p3.y + 0.5 * y3);
	//	circle_vec.push_back(getCircle(p1new, p2new, p3new));
	//					}
	//circle_vec.push_back(getCircleLinear(vp, i_head, len));
	return true;
}
//TODO
void CircleDetector::setInfo(vector<CPoint>& vp, int i_head, int len, vector<CPointInfo>& vi)
{
	const int N = (int)vp.size();
	for (int i = 0; i < len; ++i)
	{
		int index = getIndex(i_head + i, N);
		if (vi[index].type != CPointType::UNKOWN) resetInfo(vi, index);
		vi[index].type = CPointType::CIRCLE;
		vi[index].i_head = i_head;
		vi[index].len = len;
		vi[index].corner = false;
	}
	vi[getIndex(i_head, N)].corner = true;
	vi[getIndex(i_head + len - 1, N)].corner = true;
}
CircleDetector::CircleInfo CircleDetector::getCircle(vector<CPoint>& vp, int i_head, int len)
{
	//check input output
	const int N = (int)vp.size();
	const int i_back = i_head + len - 1;
	int left = i_head%N;
	int mid = ((i_head + i_back) / 2) % N;
	int right = i_back%N;

	CircleInfo circle;
	if (left == mid || mid == right || right == left)
		return circle;

	CPoint2d p1(vp[left]);
	CPoint2d p2(vp[mid]);
	CPoint2d p3(vp[right]);
	circle = getCircle(p1, p2, p3);
	return circle;
}
CircleDetector::CircleInfo CircleDetector::getCircle(CPoint2d& p1, CPoint2d& p2, CPoint2d& p3)
{
	double x1 = p1.x;
	double x2 = p2.x;
	double x3 = p3.x;

	double y1 = p1.y;
	double y2 = p2.y;
	double y3 = p3.y;

	CircleInfo circle;

	double x, y;
	x = (x1*x1 + y1*y1)*(y2 - y3) + (x2*x2 + y2*y2)*(y3 - y1) + (x3*x3 + y3*y3)*(y1 - y2);
	x /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

	y = (x1*x1 + y1*y1)*(x3 - x2) + (x2*x2 + y2*y2)*(x1 - x3) + (x3*x3 + y3*y3)*(x2 - x1);
	y /= (2 * (x1*(y2 - y3) - y1*(x2 - x3) + x2*y3 - x3*y2));

	circle.center.x = x;
	circle.center.y = y;
	circle.radius = sqrt((x - x1)*(x - x1) + (y - y1)*(y - y1));
	return circle;
}
CircleDetector::CircleInfo CircleDetector::getCircleLinear(vector<CPoint>& vp, int i_head, int len)
{
	const int N = (int)vp.size();
	vector<CPoint> points;
	for (int i = 0; i < len; ++i)
		points.push_back(vp[getIndex(i_head + i, N)]);
	return getCircleLinear(points);
}
CircleDetector::CircleInfo CircleDetector::getCircleLinear(vector<CPoint>& points)
{
	//b = (A.t()*A).inv()*A.t()*Y;
	const int N = (int)points.size();
	assert(N >= 3);
	vector<vector<double> > A(N);//n*3
	vector<vector<double> > Y(N);
	for (int i = 0; i < N; ++i){
		A[i].resize(3);
		A[i][0] = 1;
		A[i][1] = points[i].y;
		A[i][2] = points[i].x;
		Y[i].resize(1);
		Y[i][0] = -points[i].x*points[i].x - points[i].y*points[i].y;
	}
	vector<vector<double> > AT = matrixTrans(A);
	vector<vector<double> > ATA = matrixMultiply(AT, A);
	vector<vector<double> > ATAI = matrixInvert(ATA);
	vector<vector<double> > ATAI_AT = matrixMultiply(ATAI, AT);
	vector<vector<double> > b = matrixMultiply(ATAI_AT, Y);

	double AA = b[2][0];
	double BB = b[1][0];
	double CC = b[0][0];

	CircleInfo circle;
	circle.center.x = AA / (-2.0);
	circle.center.y = BB / (-2.0);
	circle.radius = sqrt(circle.center.x*circle.center.x + circle.center.y*circle.center.y - CC);
	return circle;
}
vector<vector<double> > CircleDetector::matrixMultiply(vector<vector<double> >A, vector<vector<double> >B)
{
	const int M = (int)A.size();
	const int K = (int)A[0].size();
	const int N = (int)B[0].size();
	assert(A[0].size() == B.size());
	vector<vector<double> > C(M);
	for (int m = 0; m < M; ++m){
		C[m].resize(N);
		for (int n = 0; n < N; ++n){
			double sum = 0;
			for (int k = 0; k < K; ++k)
				sum += A[m][k] * B[k][n];
			C[m][n] = sum;
		}
	}
	return C;
}
vector<vector<double> > CircleDetector::matrixInvert(vector<vector<double> >A)
{
	const int n = (int)A.size();
	double* data = new double[n*n];
	for (int i = 0; i < n; ++i)
		for (int j = 0; j < n; ++j)
			data[i*n + j] = A[i][j];
	for (int i = 1; i < n; i++) data[i] /= data[0]; // normalize row 0
	for (int i = 1; i < n; i++)  {
		for (int j = i; j < n; j++)  { // do a column of L
			double sum = 0.0;
			for (int k = 0; k < i; k++)
				sum += data[j*n + k] * data[k*n + i];
			data[j*n + i] -= sum;
		}
		for (int j = i + 1; j < n; j++)  {  // do a row of U
			double sum = 0.0;
			for (int k = 0; k < i; k++)
				sum += data[i*n + k] * data[k*n + j];
			data[i*n + j] =
				(data[i*n + j] - sum) / data[i*n + i];
		}
	}
	for (int i = 0; i < n; i++)  // invert L
		for (int j = i; j < n; j++)  {
		double x = 1.0;
		if (i != j) {
			x = 0.0;
			for (int k = i; k < j; k++)
				x -= data[j*n + k] * data[k*n + i];
		}
		data[j*n + i] = x / data[j*n + j];
		}
	for (int i = 0; i < n; i++)   // invert U
		for (int j = i; j < n; j++)  {
		if (i == j) continue;
		double sum = 0.0;
		for (int k = i; k < j; k++)
			sum += data[k*n + j] * ((i == k) ? 1.0 : data[i*n + k]);
		data[i*n + j] = -sum;
		}
	for (int i = 0; i < n; i++)   // final inversion
		for (int j = 0; j < n; j++)  {
		double sum = 0.0;
		for (int k = ((i > j) ? i : j); k < n; k++)
			sum += ((j == k) ? 1.0 : data[j*n + k])*data[k*n + i];
		data[j*n + i] = sum;
		}
	vector<vector<double> > B(n);
	for (int i = 0; i < n; ++i){
		B[i].resize(n);
		for (int j = 0; j < n; ++j)
			B[i][j] = data[i*n + j];
	}
	delete data;
	return B;
}
vector<vector<double> > CircleDetector::matrixTrans(vector<vector<double> >A)
{
	const int m = (int)A.size();
	const int n = (int)A[0].size();
	vector<vector<double> > B(n);
	for (int i = 0; i < n; ++i){
		B[i].resize(m);
		for (int j = 0; j < m; ++j)
			B[i][j] = A[j][i];
	}
	return B;
}
static void printMatrix(vector<vector<double> > A)
{
	for (int i = 0; i < (int)A.size(); ++i){
		for (int j = 0; j < (int)A[i].size(); ++j){
			cout << A[i][j] << "\t";
		}
		cout << endl;
	}
}
void CircleDetector::testMatrix()
{
	cout << "Test Invert" << endl;
	int n = 2;
	vector<vector<double> > A(n);
	for (int i = 0; i < n; ++i){
		A[i].resize(n);
		for (int j = 0; j < n; ++j)
			A[i][j] = rand()%5;
	}
	vector<vector<double> > AI = matrixInvert(A);
	vector<vector<double> > E = matrixMultiply(A, AI);
	cout << "A" << endl;
	printMatrix(A);
	cout << "AI" << endl;
	printMatrix(AI);
	cout << "A*AI" << endl;
	printMatrix(E);

	cout << "Test Trans" << endl;
	int m = 3;
	vector<vector<double> > B(n);
	for (int i = 0; i < n; ++i){
		B[i].resize(m);
		for (int j = 0; j < m; ++j)
			B[i][j] = rand() % 5;
	}
	vector<vector<double> > BT = matrixTrans(B);
	cout << "B" << endl;
	printMatrix(B);
	cout << "BT" << endl;
	printMatrix(BT);

	cout << "Test matrixMultiply" << endl;
	vector<vector<double> > BBT = matrixMultiply(B, BT);
	cout << "BBT" << endl;
	printMatrix(BBT);
}