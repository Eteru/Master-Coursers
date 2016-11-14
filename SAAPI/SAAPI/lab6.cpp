
#include "lab6.hpp"

namespace lab6
{
	void ex1(void)
	{
		for (char c = 'a'; c <= 'f'; ++c) {
			std::string fname = std::string("imgs\\test1") + c + "\.jpg";
			cv::Mat img = cv::imread(fname, CV_LOAD_IMAGE_GRAYSCALE);

			utils::showGrayHistogram(img, fname);
		}

		cv::waitKey(0);
		
	}

	void ex2(void)
	{
		// cv::Mat img = cv::imread("imgs\\circ.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat noise = cv::imread("imgs\\circ1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat maImg(noise.rows, noise.cols, CV_8U), geomImg(noise.rows, noise.cols, CV_8U);

		utils::maFilter(noise, maImg);
		utils::geomFilter(noise, geomImg);

		utils::showThreeMats(noise, maImg, geomImg, CV_8U, "ORIGINAL / MA / GEOMETRIC");

		cv::waitKey(0);
	}

	void ex3(void)
	{
		cv::Mat noise1 = cv::imread("imgs\\circ2.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat noise2 = cv::imread("imgs\\circ3.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat qImgP1(noise1.rows, noise1.cols, CV_8U), qImgP2(noise1.rows, noise1.cols, CV_8U),
			qImgN1(noise1.rows, noise1.cols, CV_8U), qImgN2(noise1.rows, noise1.cols, CV_8U);

		utils::counterArmonicFilter(noise1, qImgP1, 1.5f);
		utils::counterArmonicFilter(noise2, qImgP2, 1.5f);
		utils::counterArmonicFilter(noise1, qImgN1, -1.5f);
		utils::counterArmonicFilter(noise2, qImgN2, -1.5f);

		utils::showThreeMats(noise1, qImgP1, qImgN1, CV_8U, "ORIGINAL1 / Positive / Negative");
		utils::showThreeMats(noise2, qImgP2, qImgN2, CV_8U, "ORIGINAL2 / Positive / Negative");

		cv::waitKey(0);
	}

	void ex4(void)
	{
		cv::Mat noise = cv::imread("imgs\\circ4.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat maImg(noise.rows, noise.cols, CV_8U), medianImg(noise.rows, noise.cols, CV_8U);

		utils::maFilter(noise, maImg);
		utils::medianFilter(noise, medianImg);

		utils::showThreeMats(noise, maImg, medianImg, CV_8U, "ORIGINAL / MA / MEDIAN");

		cv::waitKey(0);
	}

}