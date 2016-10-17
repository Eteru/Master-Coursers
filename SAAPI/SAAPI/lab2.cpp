
#include "lab2.hpp"
namespace lab2
{
	void myDFT(cv::Mat &inIMG, const int &sz, cv::Mat &real, cv::Mat &imag, cv::Mat &magn)
	{
		for (int iPX = 0; iPX < sz; ++iPX) {
			for (int jPX = 0; jPX < sz; ++jPX) {
				real.at<double>(iPX, jPX) = 0;
				imag.at<double>(iPX, jPX) = 0;

				for (int i = 0; i < sz; ++i)
					for (int j = 0; j < sz; ++j) {
						real.at<double>(iPX, jPX) += (inIMG.at<double>(i, j) * cos(2 * PI * (1.0 * iPX * i / sz + 1.0 * jPX * j / sz)));
						imag.at<double>(iPX, jPX) -= (inIMG.at<double>(i, j) * sin(2 * PI * (1.0 * iPX * i / sz + 1.0 * jPX * j / sz)));
					}

				real.at<double>(iPX, jPX) /= sz*sz;
				imag.at<double>(iPX, jPX) /= sz*sz;
				magn.at<double>(iPX, jPX) = sqrtl(real.at<double>(iPX, jPX) * real.at<double>(iPX, jPX) + imag.at<double>(iPX, jPX) * imag.at<double>(iPX, jPX));
			}
		}
	}

	void myIDFT(cv::Mat &inIMG, const int &sz, cv::Mat &real, cv::Mat &imag, cv::Mat &magn)
	{
		for (int iPX = 0; iPX < sz; ++iPX) {
			for (int jPX = 0; jPX < sz; ++jPX) {
				double realS = 0, imagS = 0;

				for (int i = 0; i < sz; ++i)
					for (int j = 0; j < sz; ++j) {
						realS += (real.at<double>(i, j) * cos(2 * PI * (1.0 * iPX * i / sz + 1.0 * jPX * j / sz)));
						imagS += (imag.at<double>(i, j) * sin(2 * PI * (1.0 * iPX * i / sz + 1.0 * jPX * j / sz)));
					}

				magn.at<double>(iPX, jPX) = realS - imagS;
			}
		}
	}

	void openCVDFT(cv::Mat &I, cv::Mat &magI)
	{
		cv::Mat padded;
		int m = cv::getOptimalDFTSize(I.rows);
		int n = cv::getOptimalDFTSize(I.cols);

		cv::copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

		cv::Mat planes[] = { cv::Mat_<double>(padded), cv::Mat::zeros(padded.size(), CV_64FC1) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);
		cv::magnitude(planes[0], planes[1], planes[0]);
		magI = planes[0];

		magI += cv::Scalar::all(1);
		cv::log(magI, magI);
		cv::normalize(magI, magI, 0, 1, CV_MINMAX);
	}

	void openCVIDFT(cv::Mat &I, cv::Mat &magI)
	{
		static int check = false;
		cv::Mat padded;
		int m = cv::getOptimalDFTSize(I.rows);
		int n = cv::getOptimalDFTSize(I.cols);

		cv::copyMakeBorder(I, padded, 0, m - I.rows, 0, n - I.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));

		cv::Mat planes[] = { cv::Mat_<double>(padded), cv::Mat::zeros(padded.size(), CV_64FC1) };
		cv::Mat complexI;
		cv::merge(planes, 2, complexI);

		cv::dft(complexI, complexI);

		cv::split(complexI, planes);
		cv::magnitude(planes[0], planes[1], planes[0]);
		magI = planes[0];

		magI += cv::Scalar::all(1);
		cv::log(magI, magI);
		cv::normalize(magI, magI, 0, 1, CV_MINMAX);

		cv::namedWindow("CVDFT", cv::WINDOW_AUTOSIZE);
		cv::imshow("CVDFT", magI);

		cv::Mat inverseTransform;
		cv::dft(complexI, inverseTransform, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
		cv::normalize(inverseTransform, inverseTransform, 0, 1, CV_MINMAX);
		imshow("Reconstructed", inverseTransform);
	}

	void ex1_2(void)
	{
		const static int sz = 64;
		const static float rMod = 0.3f;
		const static float gMod = 0.59f;
		const static float bMod = 0.11f;
		std::string vertPath = "imgs\\vertical.bmp",
		horizPath = "imgs\\horizontal.bmp",
		squaresPath = "imgs\\rectangles.bmp";


		cv::Mat vert = cv::imread(vertPath, CV_LOAD_IMAGE_COLOR);
		cv::Mat horiz = cv::imread(horizPath, CV_LOAD_IMAGE_COLOR);
		cv::Mat squares = cv::imread(squaresPath, CV_LOAD_IMAGE_COLOR);

		// Create gray images
		cv::Mat vertGray(sz, sz, CV_64FC1),
		horizGray(sz, sz, CV_64FC1),
		squaresGray(sz, sz, CV_64FC1);

		for (int i = 0; i < sz; ++i) {
			for (int j = 0; j < sz; ++j) {
				cv::Vec3b pixel = vert.at<cv::Vec3b>(i, j);
				vertGray.at<double>(i, j) = pixel.val[0] * bMod + pixel.val[1] * gMod + pixel.val[2] * rMod;

				pixel = horiz.at<cv::Vec3b>(i, j);
				horizGray.at<double>(i, j) = pixel.val[0] * bMod + pixel.val[1] * gMod + pixel.val[2] * rMod;

				pixel = squares.at<cv::Vec3b>(i, j);
				squaresGray.at<double>(i, j) = pixel.val[0] * bMod + pixel.val[1] * gMod + pixel.val[2] * rMod;
			}
		}

		cv::namedWindow("GrayscaleVert", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("GrayscaleHoriz", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("GrayscaleRect", cv::WINDOW_AUTOSIZE);
		cv::imshow("GrayscaleVert", vertGray);
		cv::imshow("GrayscaleHoriz", horizGray);
		cv::imshow("GrayscaleRect", squaresGray);

		// Fourier
		cv::Mat magn1(sz, sz, CV_64FC1),
		magn2(sz, sz, CV_64FC1),
		magn3(sz, sz, CV_64FC1);

		openCVDFT(vertGray, magn1);
		cv::imwrite("imgs\\vertCV.bmp", magn1 * 255);

		openCVDFT(horizGray, magn2);
		cv::imwrite("imgs\\horizCV.bmp", magn2 * 255);

		openCVDFT(squaresGray, magn3);
		cv::imwrite("imgs\\rectanglesCV.bmp", magn3 * 255);

		cv::namedWindow("OpenCVDFTVert", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("OpenCVDFTHoriz", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("OpenCVDFTRect", cv::WINDOW_AUTOSIZE);
		cv::imshow("OpenCVDFTVert", magn1);
		cv::imshow("OpenCVDFTHoriz", magn2);
		cv::imshow("OpenCVDFTRect", magn3);

		cv::Mat magn4(sz, sz, CV_64FC1),
		magn5(sz, sz, CV_64FC1),
		magn6(sz, sz, CV_64FC1),
		real4(sz, sz, CV_64FC1), imag4(sz, sz, CV_64FC1),
		real5(sz, sz, CV_64FC1), imag5(sz, sz, CV_64FC1),
		real6(sz, sz, CV_64FC1), imag6(sz, sz, CV_64FC1);
		myDFT(vertGray, sz, real4, imag4, magn4);
		cv::imwrite("imgs\\grayverticalMine.bmp", magn4 * 255);

		myDFT(horizGray, sz, real5, imag5, magn5);
		cv::imwrite("imgs\\grayhorizontalMine.bmp", magn5 * 255);

		myDFT(squaresGray, sz, real6, imag6, magn6);
		cv::imwrite("imgs\\grayrectanglesMine.bmp", magn6 * 255);

		cv::namedWindow("MyDFTVert", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("MyDFTHoriz", cv::WINDOW_AUTOSIZE);
		cv::namedWindow("MyDFTRect", cv::WINDOW_AUTOSIZE);
		cv::imshow("MyDFTVert", magn4);
		cv::imshow("MyDFTHoriz", magn5);
		cv::imshow("MyDFTRect", magn6);

		cv::waitKey(0);
	}

	void ex3_4(void)
	{
		const static int sz = 64;
		const static float rMod = 0.3f;
		const static float gMod = 0.59f;
		const static float bMod = 0.11f;

		// Ex 3 & 4
		cv::Mat img = cv::imread("imgs\\bear.bmp", CV_LOAD_IMAGE_COLOR);
		cv::Mat subIMG = img(cv::Rect(320, 200, 64, 64));

		cv::Mat subIMGGray(sz, sz, CV_64FC1);

		for (int i = 0; i < sz; ++i)
			for (int j = 0; j < sz; ++j) {
				cv::Vec3b pixel = subIMG.at<cv::Vec3b>(i, j);
				subIMGGray.at<double>(i, j) = pixel.val[0] * bMod + pixel.val[1] * gMod + pixel.val[2] * rMod;
			}

		cv::normalize(subIMGGray, subIMGGray, 0, 1, CV_MINMAX);
		cv::namedWindow("Gray img", cv::WINDOW_AUTOSIZE);
		cv::imshow("Gray img", subIMGGray);

		// DFT
		cv::Mat magn(sz, sz, CV_64FC1), real(sz, sz, CV_64FC1), imag(sz, sz, CV_64FC1), magnCV(sz, sz, CV_64FC1);
		myDFT(subIMGGray, sz, real, imag, magn);

		cv::namedWindow("MyDFT", cv::WINDOW_AUTOSIZE);
		cv::imshow("MyDFT", magn);

		// IDFT
		cv::Mat imagn(sz, sz, CV_64FC1), ireal(sz, sz, CV_64FC1), iimag(sz, sz, CV_64FC1), imagnCV(sz, sz, CV_64FC1);
		myIDFT(magn, sz, real, imag, imagn);

		cv::normalize(imagn, imagn, 0, 1, CV_MINMAX);
		cv::namedWindow("MyIDFT", cv::WINDOW_AUTOSIZE);
		cv::imshow("MyIDFT", imagn);

		openCVIDFT(subIMGGray, magnCV);

		cv::waitKey(0);
	}
}