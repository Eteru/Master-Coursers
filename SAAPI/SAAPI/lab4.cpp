
#include "utils.hpp"
#include "lab4.hpp"

#include <iostream>

namespace lab4
{
	void ex1(void)
	{
		cv::Mat img = cv::imread("imgs\\bear.bmp", CV_LOAD_IMAGE_COLOR);
		cv::Mat imgGray(img.rows, img.cols, CV_64FC1);

		cv::cvtColor(img, imgGray, CV_BGR2GRAY);

		utils::showGrayHistogram(imgGray, "Gray Histogram");

		cv::waitKey(0);

	}

	void ex2(void)
	{
		cv::Mat img = cv::imread("imgs\\bear.bmp", CV_LOAD_IMAGE_COLOR);
		cv::Mat imgGray(img.rows, img.cols, CV_64FC1), imgGrayEqualised(img.rows, img.cols, CV_64FC1);

		// Make it gray
		cv::cvtColor(img, imgGray, CV_BGR2GRAY);

		// Apply Histogram Equalization
		utils::equaliseHistogram(imgGray, imgGrayEqualised);

		cv::namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
		cv::imshow("Gray Image", imgGray);
		cv::namedWindow("Gray Equlised Image", CV_WINDOW_AUTOSIZE);
		cv::imshow("Gray Equlised Image", imgGrayEqualised);

		utils::showBGRHistogram(img, "BGR Histogram");
		utils::showGrayHistogram(imgGray, "Gray Histogram");
		utils::showGrayHistogram(imgGrayEqualised, "Gray Equalised Histogram");

		cv::waitKey(0);
	}

	void ex3(void)
	{
		cv::Mat img = cv::imread("imgs\\bear.bmp", CV_LOAD_IMAGE_COLOR);
		cv::Mat imgGray(img.rows, img.cols, CV_64FC1), imgGrayEqualised(img.rows, img.cols, CV_64FC1);

		// Make it gray
		cv::cvtColor(img, imgGray, CV_BGR2GRAY);

		// Apply Histogram Equalization
		utils::equaliseHistogram(imgGray, imgGrayEqualised);

		// Wanted histogram
		std::vector<int> wantedHistogram(256, 0);
		utils::imHist(imgGrayEqualised, wantedHistogram);
		//std::reverse(wantedHistogram.begin(), wantedHistogram.end());

		// Generate cumulative frequency histogram
		std::vector<int> cumhistogram(256, 0);
		utils::cumHist(wantedHistogram, cumhistogram);

		// Caluculate the size of image
		int size = img.rows * img.cols;
		float alpha = static_cast<float>(255.0 / size);

		// Scale the histogram
		std::vector<int> Sk(256, 0);
		for (int i = 0; i < 256; ++i)
			Sk[i] = cvRound(static_cast<double>(cumhistogram[i] * alpha));

		cv::Mat outimg(img.rows, img.cols, CV_8U);

		for (int x = 0; x < img.rows; ++x)
			for (int y = 0; y < img.cols; ++y)
				outimg.at<uchar>(x, y) = static_cast<uchar>(Sk[imgGray.at<uchar>(x, y)]);

		utils::showGrayHistogram(imgGrayEqualised, "Wanted histogram");
		utils::showGrayHistogram(outimg, "Obtained Histogram");

		cv::waitKey(0);
	}
}