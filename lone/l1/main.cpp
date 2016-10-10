
#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <iostream>
#include <Windows.h>

int main(void)
{
	// Ex 1
	std::cout << "Ex 1:" << std::endl;
	int blockSize = 50;
	int imageSize = blockSize * 8;
	cv::Mat chessBoard(imageSize, imageSize, CV_8UC3, cv::Scalar::all(0));
	unsigned char color = 0;

	for (int i = 0; i<imageSize; i = i + blockSize) {
		color = ~color;
		for (int j = 0; j<imageSize; j = j + blockSize) {
			cv::Mat ROI = chessBoard(cv::Rect(i, j, blockSize, blockSize));
			ROI.setTo(cv::Scalar::all(color));
			color = ~color;
		}
	}

	cv::namedWindow("Chess board", CV_WINDOW_AUTOSIZE);
	cv::imshow("Chess board", chessBoard);

	cv::RNG rng(12345);
	cv::Mat coloredChessBoard = cv::Mat::zeros(imageSize, imageSize, CV_8UC3);
	for (int i = 0; i<imageSize; i = i + blockSize) {
		for (int j = 0; j < imageSize; j = j + blockSize) {
			cv::Scalar color = cv::Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
			cv::Mat ROI = coloredChessBoard(cv::Rect(i, j, blockSize, blockSize));
			ROI.setTo(color);
		}
	}

	cv::namedWindow("Colored Chess board", CV_WINDOW_AUTOSIZE);
	cv::imshow("Colored Chess board", coloredChessBoard);

	std::cout << "Done." << std::endl << std::endl;

	// Ex 2
	std::cout << "Ex 2:" << std::endl;
	cv::Mat dogePhoto = cv::imread("imgs\\doge.png", CV_LOAD_IMAGE_COLOR);
	if (!dogePhoto.data) {
		std::cout << "Problem loading image for ex 2" << std::endl;
	}
	else {
		cv::namedWindow("Very doge, much wow", CV_WINDOW_AUTOSIZE);
		cv::imshow("Very doge, much wow", dogePhoto);

		for (int i = 0; i < dogePhoto.rows; ++i)
			for (int j = 0; j < dogePhoto.cols; ++j) {
				cv::Vec4b pixel = dogePhoto.at<cv::Vec4b>(i, j);
				std::cout << "Pixel(" << i << ", " << j <<"): "
							<< (int)pixel[0] << " " << (int)pixel[1] << " "
							<< (int)pixel[2] << " alpha: " << (int)pixel[3]
							<< std::endl;
			}

		std::cout << "Done." << std::endl << std::endl;
	}

	// Ex 3
	std::cout << "Ex 3:" << std::endl;
	cv::Mat pixelsPhoto = cv::imread("imgs\\pixels.bmp", CV_LOAD_IMAGE_COLOR);
	if (!pixelsPhoto.data) {
		std::cout << "Problem loading image for ex 2" << std::endl;
	}
	else {
		cv::namedWindow("Pixels", CV_WINDOW_AUTOSIZE);
		cv::imshow("Pixels", pixelsPhoto);

		for (int i = 0; i < pixelsPhoto.rows; ++i)
			for (int j = 0; j < pixelsPhoto.cols; ++j) {
				cv::Vec3b pixel = pixelsPhoto.at<cv::Vec3b>(i, j);
				std::cout << "Pixel(" << i << ", " << j << "): "
					<< (int)pixel[0] << " " << (int)pixel[1] << " "
					<< (int)pixel[2] << std::endl;
			}
		cv::Size sz = pixelsPhoto.size();

		std::cout << "Width: " << sz.width << std::endl;
		std::cout << "Height: " << sz.height << std::endl;
		std::cout << "Bits / pixel: " << pixelsPhoto.type() << std::endl;
		std::cout << "Done." << std::endl << std::endl;
	}

	// Ex 4
	std::cout << "Ex 4:" << std::endl;
	cv::Mat modifiedPixelsPhoto;
	pixelsPhoto.copyTo(modifiedPixelsPhoto);

	for (int i = 0; i < modifiedPixelsPhoto.rows; ++i)
		for (int j = 0; j < modifiedPixelsPhoto.cols; ++j) {
			cv::Vec3b pixel = modifiedPixelsPhoto.at<cv::Vec3b>(i, j);
			pixel.val[0] = rng.uniform(0, 255);
			pixel.val[1] = rng.uniform(0, 255);
			pixel.val[2] = rng.uniform(0, 255);

			modifiedPixelsPhoto.at<cv::Vec3b>(i, j) = pixel;
		}

	if (!cv::imwrite("imgs\\modified_pixels.bmp", modifiedPixelsPhoto))
		std::cout << "Failed saving file for ex 4." << std::endl;

	cv::namedWindow("Modified Pixels", CV_WINDOW_AUTOSIZE);
	cv::imshow("Modified Pixels", modifiedPixelsPhoto);

	std::cout << "Done." << std::endl << std::endl;

	cv::waitKey(0);
	return 0;
}