
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
}