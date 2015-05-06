#pragma once

#include <vector>
#include <string>
#include "opencv2/opencv.hpp"
using namespace std;
using namespace cv;
class CornerDetectorBase;
class Commander;

class Tester
{
public:
	void test(
		string& path_input,
		vector<string>& list,
		int id_head,
		int id_back,
		CornerDetectorBase* classifier,
		string& path_oputput);

private:
	void testOne(
		string name_input,
		CornerDetectorBase* classifier,
		string name_output);


};
