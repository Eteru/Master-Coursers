#pragma once

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include <cmath>

#define PI 3.14159265

namespace utils
{
	const static float rMod = 0.3f;
	const static float gMod = 0.59f;
	const static float bMod = 0.11f;

	void makeItGray(cv::Mat &colored, cv::Mat &gray);
	void showThreeMats(cv::Mat &I1, cv::Mat &I2, cv::Mat &I3, int dataType, std::string name);

	void showGrayHistogram(const cv::Mat &img, const std::string &windowName);
	void showBGRHistogram(const cv::Mat &img, const std::string &windowName);

	void imHist(const cv::Mat &image, std::vector<int> &histogram);
	void cumHist(const std::vector<int> &histogram, std::vector<int> &cumhistogram);
	void equaliseHistogram(const cv::Mat &img, cv::Mat &outimg);
}
