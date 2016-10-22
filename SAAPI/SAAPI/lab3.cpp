
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>

#include "utils.hpp"
#include "lab3.hpp"

namespace lab3
{
	void cosTransformation(cv::Mat &in, cv::Mat &out)
	{
		int doubleWidth = 2 * in.cols;
		int doubleHeight = 2 * in.rows;
		float factor = float(2.0 / sqrt(in.cols * in.rows));
		float revSqrtOfTwo = float(1.0 / sqrt(2));

		for (int iOut = 0; iOut < out.rows; ++iOut)
			for (int jOut = 0; jOut < out.cols; ++jOut) {
				out.at<double>(iOut, jOut) = 0;

				for (int iIn = 0; iIn < in.rows; ++iIn)
					for (int jIn = 0; jIn < in.cols; ++jIn)	{
						out.at<double>(iOut, jOut) += in.at<double>(iIn, jIn) * cos((2.0 * iIn + 1) / doubleWidth * iOut * PI) * cos((2.0 * jIn + 1) / doubleHeight * jOut * PI);
					}

				out.at<double>(iOut, jOut) *= factor;

				if (iOut == 0)
					out.at<double>(iOut, jOut) *= revSqrtOfTwo;
				if (jOut == 0)
					out.at<double>(iOut, jOut) *= revSqrtOfTwo;
			}
	}

	void cosTransformationInverse(cv::Mat &in, cv::Mat &out)
	{
		int doubleWidth = 2 * in.cols;
		int doubleHeight = 2 * in.rows;
		float factor = float(2.0 / sqrt(in.cols * in.rows));
		float revSqrtOfTwo = float(1.0 / sqrt(2));

		for (int iOut = 0; iOut < out.rows; ++iOut)
			for (int jOut = 0; jOut < out.cols; ++jOut) {
				out.at<double>(iOut, jOut) = 0;

				for (int iIn = 0; iIn < in.rows; ++iIn)
					for (int jIn = 0; jIn < in.cols; ++jIn) {
						double step = in.at<double>(iIn, jIn) * cos((2.0 * iOut + 1) / doubleWidth * iIn * PI) * cos((2.0 * jOut + 1) / doubleHeight * jIn * PI);

						if (iIn == 0)
							step *= revSqrtOfTwo;
						if (jIn == 0)
							step *= revSqrtOfTwo;

						out.at<double>(iOut, jOut) += step;
					}

				out.at<double>(iOut, jOut) *= factor;
			}
	}

	void ex1_2(void)
	{
		std::string vertPath = "imgs\\vertical.bmp",
					horizPath = "imgs\\horizontal.bmp",
					squaresPath = "imgs\\rectangles.bmp";

		cv::Mat vert = cv::imread(vertPath, CV_LOAD_IMAGE_COLOR);
		cv::Mat horiz = cv::imread(horizPath, CV_LOAD_IMAGE_COLOR);
		cv::Mat squares = cv::imread(squaresPath, CV_LOAD_IMAGE_COLOR);

		cv::Mat vertGray(vert.rows, vert.cols, CV_64FC1),
				horizGray(horiz.rows, horiz.cols, CV_64FC1),
				squaresGray(squares.rows, squares.cols, CV_64FC1);

		utils::makeItGray(vert, vertGray);
		utils::makeItGray(horiz, horizGray);
		utils::makeItGray(squares, squaresGray);

		cv::Mat vertGrayCos(vert.rows, vert.cols, CV_64FC1),
				horizGrayCos(horiz.rows, horiz.cols, CV_64FC1),
				squaresGrayCos(squares.rows, squares.cols, CV_64FC1);

		cosTransformation(vertGray, vertGrayCos);
		cosTransformation(horizGray, horizGrayCos);
		cosTransformation(squaresGray, squaresGrayCos);

		cv::imwrite("imgs\\vertCos.bmp", vertGrayCos);
		cv::imwrite("imgs\\horizCos.bmp", horizGrayCos);
		cv::imwrite("imgs\\squaresCos.bmp", squaresGrayCos);

		cv::Mat vertGrayCosCV(vert.rows, vert.cols, CV_64FC1),
				horizGrayCosCV(horiz.rows, horiz.cols, CV_64FC1),
				squaresGrayCosCV(squares.rows, squares.cols, CV_64FC1);

		cv::dct(vertGray, vertGrayCosCV, CV_DXT_FORWARD);
		cv::dct(horizGray, horizGrayCosCV, CV_DXT_FORWARD);
		cv::dct(squaresGray, squaresGrayCosCV, CV_DXT_FORWARD);

		cv::Mat vertGrayICos(vert.rows, vert.cols, CV_64FC1),
				horizGrayICos(horiz.rows, horiz.cols, CV_64FC1),
				squaresGrayICos(squares.rows, squares.cols, CV_64FC1);

		cosTransformationInverse(vertGrayCos, vertGrayICos);
		cosTransformationInverse(horizGrayCos, horizGrayICos);
		cosTransformationInverse(squaresGrayCos, squaresGrayICos);

		cv::Mat vertGrayICosCV(vert.rows, vert.cols, CV_64FC1),
				horizGrayICosCV(horiz.rows, horiz.cols, CV_64FC1),
				squaresGrayICosCV(squares.rows, squares.cols, CV_64FC1);

		cv::dct(vertGrayCosCV, vertGrayICosCV, CV_DXT_INVERSE);
		cv::dct(horizGrayCosCV, horizGrayICosCV, CV_DXT_INVERSE);
		cv::dct(squaresGrayCosCV, squaresGrayICosCV, CV_DXT_INVERSE);


		utils::showThreeMats(vertGray, horizGray, squaresGray, CV_64FC1, "Grayed imgs");
		// utils::showThreeMats(vertGrayCos, horizGrayCos, horizGrayCos, CV_64FC1, "Cosed imgs");
		utils::showThreeMats(vertGrayCosCV, horizGrayCosCV, squaresGrayCosCV, CV_64FC1, "CVCosed imgs");
		utils::showThreeMats(vertGrayICos, horizGrayICos, squaresGrayICos, CV_64FC1, "Inverse Cosed imgs");
		utils::showThreeMats(vertGrayICosCV, horizGrayICosCV, squaresGrayICosCV, CV_64FC1, "CVICosed imgs");
		cv::waitKey(0);
	}
}