
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
			line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(hist.at<float>(i))),
				cv::Scalar(255, 255, 255), 2, 8, 0);
		}

		// Display
		cv::namedWindow(windowName, CV_WINDOW_AUTOSIZE);
		imshow(windowName, histImage);
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
		calcHist(&bgr_planes[0], 1, 0, cv::Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&bgr_planes[1], 1, 0, cv::Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate);
		calcHist(&bgr_planes[2], 1, 0, cv::Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate);

		// Draw the histograms for B, G and R
		int hist_w = 512; int hist_h = 400;
		int bin_w = cvRound((double)hist_w / histSize);

		cv::Mat histImage(hist_h, hist_w, CV_64FC3, cv::Scalar(0, 0, 0));

		// Normalize the result to [ 0, histImage.rows ]
		normalize(b_hist, b_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(g_hist, g_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());
		normalize(r_hist, r_hist, 0, histImage.rows, cv::NORM_MINMAX, -1, cv::Mat());

		// Draw for each channel
		for (int i = 1; i < histSize; i++)
		{
			line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(b_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(b_hist.at<float>(i))),
				cv::Scalar(255, 0, 0), 2, 8, 0);
			line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(g_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(g_hist.at<float>(i))),
				cv::Scalar(0, 255, 0), 2, 8, 0);
			line(histImage, cv::Point(bin_w*(i - 1), hist_h - cvRound(r_hist.at<float>(i - 1))),
				cv::Point(bin_w*(i), hist_h - cvRound(r_hist.at<float>(i))),
				cv::Scalar(0, 0, 255), 2, 8, 0);
		}

		// Display
		cv::namedWindow("RGB Histogram", CV_WINDOW_AUTOSIZE);
		imshow("RGB Histogram", histImage);
	}
}