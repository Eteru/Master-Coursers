
#include "utils.hpp"
#include "lab4.hpp"

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
		cv::equalizeHist(imgGray, imgGrayEqualised);

		cv::namedWindow("Gray Image", CV_WINDOW_AUTOSIZE);
		imshow("Gray Image", imgGray);
		cv::namedWindow("Gray Equlised Image", CV_WINDOW_AUTOSIZE);
		imshow("Gray Equlised Image", imgGrayEqualised);

		utils::showBGRHistogram(img, "BGR Histogram");
		utils::showGrayHistogram(imgGray, "Gray Histogram");
		utils::showGrayHistogram(imgGrayEqualised, "Gray Equalised Histogram");

		cv::waitKey(0);
	}
}