
#include "utils.hpp"
#include "lab5.hpp"

namespace lab5
{
	void ex1(void)
	{
		cv::Mat noise = cv::imread("imgs\\circ4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat original = cv::imread("imgs\\circ.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat maImg(noise.rows, noise.cols, CV_8U), medianImg(noise.rows, noise.cols, CV_8U);

		utils::maFilter(noise, maImg);
		utils::medianFilter(noise, medianImg);

		utils::showThreeMats(maImg, medianImg, original, CV_8U, "MA / MEDIAN / ORIGINAL");

		cv::waitKey(0);
	}

	void ex2(void)
	{
		cv::Mat noise = cv::imread("imgs\\circ4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat original = cv::imread("imgs\\circ.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat maImg(noise.rows, noise.cols, CV_8U), kuwaharaImg(noise.rows, noise.cols, CV_8U);
		int kernel = 7;

		utils::maFilter(noise, maImg);
		utils::kuwaharaFilter(noise, kuwaharaImg, kernel);

		utils::showThreeMats(maImg, kuwaharaImg, original, CV_8U, "MA / KUWAHARA / ORIGINAL");

		cv::waitKey(0);
	}

	void ex3(void)
	{
		cv::Mat src = cv::imread("imgs\\test.jpg", CV_LOAD_IMAGE_COLOR);
		cv::Mat dstLow, dstHigh, srcGray;
		cv::cvtColor(src, srcGray, CV_BGR2GRAY);
		const int D = 230;

		utils::lowPassFilter(srcGray, dstLow, D);
		utils::highPassFilter(srcGray, dstHigh, D);

		imshow("Original", srcGray);
		imshow("Low Pass", dstLow);
		imshow("High pass", dstHigh);
		// cv::imwrite("imgs\\lp.bmp", dstLow * 255);
		// cv::imwrite("imgs\\hp.bmp", dstHigh * 255);
		cv::waitKey(0);
	}
}