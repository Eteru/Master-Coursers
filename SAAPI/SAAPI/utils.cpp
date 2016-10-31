
#include "utils.hpp"

#include <algorithm>

namespace utils
{
	void makeItGray(cv::Mat &colored, cv::Mat &gray)
	{
		for (int i = 0; i < colored.rows; ++i)
			for (int j = 0; j < colored.cols; ++j) {
				cv::Vec3b pixel = colored.at<cv::Vec3b>(i, j);
				gray.at<double>(i, j) = pixel.val[0] * bMod + pixel.val[1] * gMod + pixel.val[2] * rMod;
			}
	}

	void showThreeMats(cv::Mat &I1, cv::Mat &I2, cv::Mat &I3, int dataType, std::string name)
	{
		int dstWidth = I1.cols + I2.cols + I3.cols;
		int dstHeight = std::max(I1.rows, std::max(I2.rows, I3.rows));

		cv::Mat dst = cv::Mat(dstHeight, dstWidth, dataType, cv::Scalar(0));

		I1.copyTo(dst.rowRange(0, I1.rows).colRange(0, I1.cols));
		I2.copyTo(dst.rowRange(0, I2.rows).colRange(I1.cols, I1.cols + I2.cols));
		I3.copyTo(dst.rowRange(0, I3.rows).colRange(I1.cols + I2.cols, I1.cols + I2.cols + I3.cols));

		cv::namedWindow(name);
		cv::imshow(name, dst);
	}

	void showGrayHistogram(const cv::Mat &img, const std::string &windowName)
	{
		// Establish the number of bins
		int histSize = 256;

		// Set the range
		float range[] = { 0, 256 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		cv::Mat hist;

		// Compute the histogram
		calcHist(&img, 1, 0, cv::Mat(), hist, 1, &histSize, &histRange, uniform, accumulate);

		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		cv::Mat histImage(hist_h, hist_w, CV_64FC1, cv::Scalar(0, 0, 0));

		// Normalize the result to [ 0, histImage.rows ]
		cv::normalize(hist, hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// Draw for each channel
		for (int i = 1; i < histSize; i++)
		{
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
				cv::Scalar(255, 255, 255), 2, 8, 0);
		}

		// Display
		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
		cv::imshow(windowName, histImage);
	}

	void showBGRHistogram(const cv::Mat &img, const std::string &windowName)
	{
		// Separate the image in 3 places ( B, G and R )
		std::vector<cv::Mat> bgr_planes;
		cv::split(img, bgr_planes);

		// Establish the number of bins
		int histSize = 256;

		// Set the ranges ( for B,G,R) )
		float range[] = { 0, 256 };
		const float* histRange = { range };

		bool uniform = true; bool accumulate = false;

		cv::Mat b_hist, g_hist, r_hist;

		// Compute the histograms:
		cv::calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		cv::calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		cv::Mat histImage(hist_h, hist_w, CV_64FC3, cv::Scalar(0, 0, 0));

		// Normalize the result to [ 0, histImage.rows ]
		cv::normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		cv::normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// Draw for each channel
		for (int i = 1; i < histSize; i++)
		{
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, 8, 0);
			cv::line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		// Display
		cv::namedWindow("RGB Histogram", CV_WINDOW_AUTOSIZE);
		cv::imshow("RGB Histogram", histImage);
	}

	void imHist(const cv::Mat &image, std::vector<int> &histogram)
	{

		// initialize all intensity values to 0
		for (int i = 0; i < 256; i++)
			histogram[i] = 0;

		// calculate the no of pixels for each intensity values
		for (int x = 0; x < image.rows; ++x)
			for (int y = 0; y < image.cols; ++y)
				histogram[(int)image.at<uchar>(x, y)]++;

	}

	void cumHist(const std::vector<int> &histogram, std::vector<int> &cumhistogram)
	{
		cumhistogram[0] = histogram[0];

		for (int i = 1; i < 256; ++i)
			cumhistogram[i] = histogram[i] + cumhistogram[i - 1];

	}

	void equaliseHistogram(const cv::Mat &img, cv::Mat &outimg)
	{
		// Generate the histogram
		std::vector<int> histogram(256, 0);
		utils::imHist(img, histogram);

		// Caluculate the size of image
		int size = img.rows * img.cols;
		float alpha = static_cast<float>(255.0 / size);

		// Calculate the probability of each intensity
		std::vector<float> PrRk(256);
		for (int i = 0; i < 256; ++i)
			PrRk[i] = static_cast<float>(histogram[i] / size);

		// Generate cumulative frequency histogram
		std::vector<int> cumhistogram(256, 0);
		utils::cumHist(histogram, cumhistogram);

		// Scale the histogram
		std::vector<int> Sk(256, 0);
		for (int i = 0; i < 256; ++i)
			Sk[i] = cvRound(static_cast<double>(cumhistogram[i] * alpha));


		// Generate the equlized histogram
		std::vector<float> PsSk(256, 0.f);
		for (int i = 0; i < 256; ++i)
			PsSk[i] = 0;

		for (int i = 0; i < 256; ++i)
			PsSk[Sk[i]] += PrRk[i];

		std::vector<int> final(256, 0);
		for (int i = 0; i < 256; ++i)
			final[i] = cvRound(PsSk[i] * 255);


		// Generate the equlized image
		outimg = img.clone();

		for (int x = 0; x < img.rows; ++x)
			for (int y = 0; y < img.cols; ++y)
				outimg.at<uchar>(x, y) = Sk[img.at<uchar>(x, y)];
	}
}