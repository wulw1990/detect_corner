#include "Tester.h"

#include <time.h>
#include <fstream>

#include "opencv2/opencv.hpp"

#include "ContourClassifierBase.h"
#include "ContourDetector.h"
#include "./show/Show.h"
#include "./system_dependent/FileDealer.h"

Tester::Tester()
{
	file_dealer = new FileDealer();
}

void Tester::TestFileList(
	const std::string& base_name,
	const std::string& list_name,
	ContourClassifierBase* classifier,
	const std::string& base_name_output)
{
	std::vector<std::string> name_list = ReadNameList(list_name);
	TestFileList(base_name, name_list, classifier, base_name_output);
}

void Tester::TestOneInListFile(
	const std::string& base_name,
	const std::string& list_name,
	ContourClassifierBase* classifier,
	const std::string& base_name_output,
	int num)
{
	std::vector<std::string> name_list = ReadNameList(list_name);
	printf("\n%d/%d begin...\n", num, (int)name_list.size());
	clock_t start_time = clock();
	std::string full_name = base_name + name_list[num];
	std::string full_name_output = base_name_output + name_list[num];
	TestOneFile(full_name, classifier, full_name_output);
	printf("%d/%d : time = %d ms\n", num, (int)name_list.size(), clock() - start_time);
}

void Tester::TestFileList(
	const std::string& base_name,
	const std::vector<std::string>& name_list,
	ContourClassifierBase* classifier,
	const std::string& base_name_output,
	const std::vector<std::string>& name_list_output)
{
	const int n_images = (int)name_list.size();

	for (int i = 0; i < n_images; ++i)
	{
		printf("\n%d/%d begin...\n", i, n_images);
		clock_t start_time = clock();
		std::string full_name = base_name + name_list[i];
		std::string full_name_output = base_name_output + name_list_output[i];
		//std::cout << "full_name_output : " << full_name_output << std::endl;
		TestOneFile(full_name, classifier, full_name_output);
		printf("%d/%d : time = %d ms\n", i, n_images, clock() - start_time);
	}
}

void Tester::TestFileList(
	const std::string& base_name,
	const std::vector<std::string>& name_list,
	ContourClassifierBase* classifier,
	const std::string& base_name_output)
{
	const int n_images = (int)name_list.size();
	
	for (int i = 0; i < n_images; ++i)
	{
		printf("\n%d/%d begin...\n", i, n_images);
		clock_t start_time = clock();
		std::string full_name = base_name + name_list[i];
		std::string full_name_output = base_name_output + name_list[i];
		TestOneFile(full_name, classifier, full_name_output);
		printf("%d/%d : time = %d ms\n", i, n_images, clock() - start_time);
	}
	
}

void Tester::TestOneFile(
	const std::string& full_name,
	ContourClassifierBase* classifier,
	const std::string& full_name_output)
{
	int location1 = (int)full_name_output.find_last_of("\\");
	if (location1 < 0 || location1 >= full_name_output.length())
		location1 = -1;
	int location2 = (int)full_name_output.find_last_of("/");
	if (location2 < 0 || location2 >= full_name_output.length())
		location2 = -1;
	int location = std::max(location1, location2);
	file_dealer->CreatDir(full_name_output.substr(0, location));
	std::cout << "dir : " << full_name_output.substr(0, location) << std::endl;
	std::cout << "location : " << location << std::endl;
	TestOneImage(full_name, classifier, full_name_output);
}

void ConvertPointFromCv_Tester(
	const std::vector<std::vector<cv::Point> >& vvpoint_cv,
	std::vector<std::vector<ContourClassifierBase::Point> >& vvpoint)
{
	//check input, output
	vvpoint.resize(vvpoint_cv.size());
	for (std::vector<int>::size_type i = 0; i < vvpoint_cv.size(); ++i)
	{
		vvpoint[i].resize(vvpoint_cv[i].size());
		for (std::vector<int>::size_type j = 0; j < vvpoint_cv[i].size(); ++j)
		{
			vvpoint[i][j].x = vvpoint_cv[i][j].x;
			vvpoint[i][j].y = vvpoint_cv[i][j].y;
		}
	}

}

void Tester::TestOneImage(
	const std::string& full_name,
	ContourClassifierBase* classifier,
	const std::string& full_name_output)
{
	//read image
	clock_t start_time = clock();
	cv::Mat img = cv::imread(full_name, 0);//gray image;
	if (img.empty())
	{
		std::cout << full_name << " can not open!" << std::endl;
		system("pause");
		exit(-1);
	}
	printf("\tfile name : %s\n", full_name.c_str());
	printf("\tread image time %d ms\n", clock() - start_time);


	std::string suffix = full_name_output.substr(full_name_output.length() - 4, 4);
	std::string prefix = full_name_output.substr(0, full_name_output.length() - 4);
	const int scale = 5;
	const int grid_rows = 1;
	const int grid_cols = 1;
	bool grid_mask[grid_rows][grid_cols] =
	{
		{ 1 }
	};
	const int patch_rows = img.rows / grid_rows;
	const int patch_cols = img.cols / grid_cols;
	int count_grid = 0;
	for (int grid_r = 0; grid_r < grid_rows; ++grid_r)
	{
		for (int grid_c = 0; grid_c < grid_rows; ++grid_c)
		{
			if (!grid_mask[grid_r][grid_c])
				continue;

			//contour detect
			start_time = clock();
			std::vector<std::vector<cv::Point> > vvpoint_cv;
			ContourDetector* contour_detector = new ContourDetector();
			contour_detector->Detect(img, vvpoint_cv);
			printf("\tr:%d c:%d contour detect time = %d ms\n", grid_r, grid_c, clock() - start_time);

			//contour classify 
			start_time = clock();
			std::vector<std::vector<ContourClassifierBase::Point> > vvpoint;
			ConvertPointFromCv_Tester(vvpoint_cv, vvpoint);
			std::vector<std::vector<ContourClassifierBase::PointInfo> > vvinfo;
			classifier->Classify(vvpoint, vvinfo);
			printf("\tr:%d c:%d contour classify  time = %d ms\n", grid_r, grid_c, clock() - start_time);

			//draw + save to disk
			start_time = clock();
			char buf[1024];
			sprintf(buf, "%s_%dr_%dc_%dR_%dC%s",
				prefix.c_str(), grid_r + 1, grid_c + 1, grid_rows, grid_cols, suffix.c_str());
			Show::DrawContours(img.size(), vvpoint, vvinfo, buf);
			printf("\tr:%d c:%d draw+save time %d ms\n", grid_r, grid_c, clock() - start_time);

		}
	}
}

std::vector<std::string> Tester::ReadNameList(std::string txt_name)
{
	std::ifstream fin(txt_name.c_str());
	std::vector<std::string> name_list;
	std::string name;
	while (fin >> name)
		name_list.push_back(name);
	return name_list;
}