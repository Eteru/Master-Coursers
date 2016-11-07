
#include "utils.hpp"

#include <iostream>
#include <algorithm>
#include <numeric>
#include <functional>

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

	void maFilter(const cv::Mat &img, cv::Mat &outimg)
	{
		const std::vector<int> dx = {-1, -1, -1, 0, 0, 0, 1, 1, 1};
		const std::vector<int> dy = {-1, 0, 1, -1, 0, 1, -1, 0, 1};
		const int sz = 9;
		const float factor = 1.f / 9;

		for (int i = 0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j) {
				int sum = 0;
				for (int k = 0; k < sz; ++k) {
					if (i + dx[k] >= 0 && i + dx[k] < img.rows &&
						j + dy[k] >= 0 && j + dy[k] < img.cols)
						sum += static_cast<int>(img.at<uchar>(i + dx[k], j + dy[k]));
				}
				outimg.at<uchar>(i, j) = static_cast<int>(sum * factor);
			}
	}

	void medianFilter(const cv::Mat &img, cv::Mat &outimg)
	{
		const std::vector<int> dx = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
		const std::vector<int> dy = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
		const int sz = 9;
		const float factor = 1.f / 9;

		for (int i = 0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j) {
				std::vector<int> medianHolder;
				for (int k = 0; k < sz; ++k) {
					if (i + dx[k] >= 0 && i + dx[k] < img.rows &&
						j + dy[k] >= 0 && j + dy[k] < img.cols)
						medianHolder.push_back(static_cast<int>(img.at<uchar>(i + dx[k], j + dy[k])));
				}
				std::sort(medianHolder.begin(), medianHolder.end());
				outimg.at<uchar>(i, j) = medianHolder[medianHolder.size() / 2];
			}
	}

	double variance(std::vector<int> values) {
		if (values.size() == 1) return 1.7e38; // Set value big enough

		double sum = std::accumulate(values.begin(), values.end(), 0.0);
		double mean = sum / values.size();

		double temp = 0;
		for (int value : values) {
			temp += (value - mean) * (value - mean);
		}

		return sqrt(temp / (values.size() - 1));
	}

	void kuwaharaFilter(const cv::Mat &img, cv::Mat &outimg, const int kernel)
	{
		const std::vector<int> dx = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };
		const std::vector<int> dy = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
		const int sz = 9;
		const float factor = 1.f / 9;

		for (int i = 0; i < img.rows; ++i)
			for (int j = 0; j < img.cols; ++j) {
				// Regions
				std::vector<int> r1, r2, r3, r4;
				double v1, v2, v3, v4;
				for (int ri = std::max(0, i - kernel); ri <= i; ++ri)
					for (int rj = std::max(0, j - kernel); rj <= j; ++rj) {
						r1.push_back(static_cast<int>(img.at<uchar>(ri, rj)));
					}

				for (int ri = std::max(0, i - kernel); ri <= i; ++ri)
					for (int rj = j; rj < std::min(img.cols, j + kernel); ++rj) {
						r2.push_back(static_cast<int>(img.at<uchar>(ri, rj)));
					}

				for (int ri = i; ri < std::min(img.rows, i + kernel); ++ri)
					for (int rj = std::max(0, j - kernel); rj <= j; ++rj) {
						r3.push_back(static_cast<int>(img.at<uchar>(ri, rj)));
					}

				for (int ri = i; ri < std::min(img.rows, i + kernel); ++ri)
					for (int rj = j; rj < std::min(img.cols, j + kernel); ++rj) {
						r4.push_back(static_cast<int>(img.at<uchar>(ri, rj)));
					}

				v1 = variance(r1);
				v2 = variance(r2);
				v3 = variance(r3);
				v4 = variance(r4);

				double minValue = std::min(v1, std::min(v2, std::min(v3, v4)));

				if (minValue - v1 <= 0.0001)
					outimg.at<uchar>(i, j) = static_cast<int>(std::accumulate(r1.begin(), r1.end(), 0.0) / r1.size());
				else if (minValue - v2 <= 0.0001)
					outimg.at<uchar>(i, j) = static_cast<int>(std::accumulate(r2.begin(), r2.end(), 0.0) / r2.size());
				else if (minValue - v3 <= 0.0001)
					outimg.at<uchar>(i, j) = static_cast<int>(std::accumulate(r3.begin(), r3.end(), 0.0) / r3.size());
				else if (minValue - v4 <= 0.0001)
					outimg.at<uchar>(i, j) = static_cast<int>(std::accumulate(r4.begin(), r4.end(), 0.0) / r4.size());

			}
	}

	void lowPassFilter(const cv::Mat &src, cv::Mat &dst, const int D)
	{
		cv::Mat local;
		src.convertTo(local, CV_32F);
		cv::Mat planes[] = { cv::Mat_<float>(local), cv::Mat::zeros(local.size(), CV_32F) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);

		for (int i = 0; i < planes[0].rows; ++i) {
			for (int j = 0; j < planes[0].cols; ++j) {
				float dist = static_cast<float>(sqrt(i*i + j*j));

				planes[0].at<float>(i, j) = dist <= D ? planes[0].at<float>(i, j) : 0.f;
				planes[1].at<float>(i, j) = dist <= D ? planes[1].at<float>(i, j) : 0.f;
			}
		}
		imshow("Low pass F", planes[0]);
		cv::Mat combined;
		cv::Mat arr[2] = { planes[0], planes[1] };
		merge(arr, 2, combined);

		cv::dft(combined, combined, cv::DFT_INVERSE | cv::DFT_SCALE);
		cv::normalize(combined, combined, 0, 1, CV_MINMAX);
		cv::split(combined, arr);

		arr[0].copyTo(dst);
	}

	void highPassFilter(const cv::Mat &src, cv::Mat &dst, const int D)
	{
		cv::Mat local;
		src.convertTo(local, CV_32F);
		cv::Mat planes[] = { cv::Mat_<float>(local), cv::Mat::zeros(local.size(), CV_32F) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);

		for (int i = 0; i < planes[0].rows; ++i) {
			for (int j = 0; j < planes[0].cols; ++j) {
				float dist = static_cast<float>(sqrt(i*i + j*j));

				planes[0].at<float>(i, j) = dist > D ? planes[0].at<float>(i, j) : 0.f;
				planes[1].at<float>(i, j) = dist > D ? planes[1].at<float>(i, j) : 0.f;
			}
		}
		imshow("High pass F", planes[0]);
		cv::Mat combined;
		cv::Mat arr[2] = { planes[0], planes[1] };
		merge(arr, 2, combined);

		cv::dft(combined, combined, cv::DFT_INVERSE | cv::DFT_SCALE);
		cv::normalize(combined, combined, 0, 1, CV_MINMAX);
		cv::split(combined, arr);

		arr[0].copyTo(dst);
	}
}