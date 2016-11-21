
#include "lab7.hpp"
#include <iostream>

namespace lab7
{
	void ex1(void)
	{
		cv::Mat src = cv::imread("imgs\\lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat dstLow = src.clone(), dstHigh = src.clone(), dstBand = src.clone(), rjctBand = src.clone();
		const int D = 230, W = 200;

		utils::lowPassFilter(src, dstLow, D);
		utils::highPassFilter(src, dstHigh, D);
		utils::rejectBandFilter(src, rjctBand, D, W);
		utils::bandPassFilter(src, dstBand, D, W);

		imshow("Original", src);
		imshow("Low", dstLow);
		imshow("High", dstHigh);
		imshow("Reject", rjctBand);
		imshow("Pass", dstBand);

		cv::waitKey(0);
	}

	void ex2(void)
	{
		cv::Mat src = cv::imread("imgs\\nasa.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat result = src.clone();

		cv::Mat local;
		src.convertTo(local, CV_32F);
		cv::Mat planes[] = { cv::Mat_<float>(local), cv::Mat::zeros(local.size(), CV_32F) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);
		imshow("Original", src);
		imshow("Spectrum", planes[0]);

		const int D = 130, W = 150;
		utils::rejectBandFilter(src, result, D, W);

		for (int i = 0; i < planes[0].rows; ++i) {
			for (int j = 0; j < planes[0].cols; ++j) {
				float dist = static_cast<float>(sqrt(i*i + j*j));
				float H = 1.f / (1 + std::pow(dist * W / (dist * dist - D * D), 2 * src.rows));

				planes[0].at<float>(i, j) = H * planes[0].at<float>(i, j);
				planes[1].at<float>(i, j) = H * planes[1].at<float>(i, j);
			}
		}

		cv::Mat combined;
		cv::Mat arr[2] = { planes[0], planes[1] };
		merge(arr, 2, combined);

		cv::dft(combined, combined, cv::DFT_INVERSE | cv::DFT_SCALE);
		cv::normalize(combined, combined, 0, 1, CV_MINMAX);
		cv::split(combined, arr);

		imshow("Reject band", arr[0]);

		cv::waitKey(0);
	}

	void ex3(void)
	{
		cv::Mat src = cv::imread("imgs\\coperta1.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat orig = cv::imread("imgs\\coperta.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		const float T = 1.f, a = 0.1f, b = 0.1f;

		cv::Mat local;
		src.convertTo(local, CV_32F);
		cv::Mat planes[] = { cv::Mat_<float>(local), cv::Mat::zeros(local.size(), CV_32F) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);

		for (int i = 0; i < planes[0].rows; ++i) {
			for (int j = 0; j < planes[0].cols; ++j) {
				float par = i * a + j * b;
				float factor = PI * par;
				float H = sinf(factor) * T / factor;

				planes[0].at<float>(i, j) = H > -std::numeric_limits<float>::max() ?
											(planes[0].at<float>(i, j) / (H * cosf(factor))) :
											planes[0].at<float>(i, j);
				planes[1].at<float>(i, j) = H > -std::numeric_limits<float>::max() ?
											(planes[1].at<float>(i, j) / (H * (-sinf(factor)))) :
											planes[1].at<float>(i, j);
			}
		}

		cv::Mat combined;
		cv::Mat arr[2] = { planes[0], planes[1] };
		merge(arr, 2, combined);

		cv::dft(combined, combined, cv::DFT_INVERSE | cv::DFT_SCALE);
		cv::normalize(combined, combined, 0, 1, CV_MINMAX);
		cv::split(combined, arr);

		imshow("coperta1.jpg", src);
		imshow("Result", arr[0]);
		imshow("coperta.jpg", orig);

		cv::waitKey(0);
	}
}
