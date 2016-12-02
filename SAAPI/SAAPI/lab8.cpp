#include <iostream>
#include "lab8.hpp"

namespace lab8
{
	void ex1(void)
	{
		cv::Mat src = cv::imread("imgs\\lena.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		cv::Mat mask = cv::imread("imgs\\masca.bmp", CV_LOAD_IMAGE_GRAYSCALE);

		cv::threshold(src, src, 150, 255, CV_THRESH_BINARY);
		imshow("test", src);
		imshow("mask", mask);

		cv::Mat res;
		cv::bitwise_and(src, mask, res);     imshow("AND", res);
		cv::bitwise_or(src, mask, res);      imshow("OR", res);
		cv::bitwise_xor(src, mask, res);     imshow("XOR", res);

		cv::waitKey(0);
	}

	void ex2(void)
	{
		cv::Mat src = cv::imread("imgs\\fractal.jpg", CV_LOAD_IMAGE_GRAYSCALE);

		cv::Mat res;
		cv::bitwise_and(src, 240, res);

		cv::imshow("original", src);
		cv::imshow("Anulare", res);
		cv::imshow("Diferenta", src-res);

		cv::equalizeHist(src, src);

		cv::imshow("Egalizare", src);

		cv::waitKey(0);
	}

	void ex3(void)
	{
		cv::Mat src = cv::imread("imgs\\moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		src.convertTo(src, CV_32F);
		float hx[] = { 0.25f, 0, -0.25f, 0.5f, 0, -0.5f, 0.25f, 0, -0.25f };
		float hy[] = {0.25f, 0.5f, 0.25f, 0.f, 0.f, 0.f, -0.25f, -0.5f, -0.25f};

		cv::Mat xKernel = cv::Mat(3, 3, CV_32F, hx);
		cv::Mat yKernel = cv::Mat(3, 3, CV_32F, hy);

		/// Generate grad_x and grad_y
		cv::Mat grad;
		cv::Mat grad_x, grad_y;
		cv::Mat abs_grad_x, abs_grad_y;

		/// X
		cv::filter2D(src, grad_x, CV_32F, xKernel);
		cv::convertScaleAbs(grad_x, abs_grad_x);
		/// Y
		cv::filter2D(src, grad_y, CV_32F, yKernel);
		cv::convertScaleAbs(grad_y, abs_grad_y);
		/// Total Gradient (approximate)
		cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

		imshow("My Sobel", grad);

		

		/// Gradient X
		cv::Sobel(src, grad_x, CV_32F, 1, 0);
		cv::convertScaleAbs(grad_x, abs_grad_x);
		/// Gradient Y
		cv::Sobel(src, grad_y, CV_32F, 0, 1);
		cv::convertScaleAbs(grad_y, abs_grad_y);
		/// Total Gradient (approximate)
		cv::addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);

		imshow("OpenCV Sobel", grad);

		cv::waitKey(0);
	}

	void ex4(void)
	{
		cv::Mat src = cv::imread("imgs\\moon.jpg", CV_LOAD_IMAGE_GRAYSCALE);
		src.convertTo(src, CV_32F);
		cv::normalize(src, src, 0, 1, CV_MINMAX);
		
		cv::Mat res;

		cv::Laplacian(src, res, CV_32F);
		cv::Mat sum;
		cv::add(src, res, sum);
		
		cv::imshow("original", src);
		cv::imshow("laplace", res);
		cv::imshow("sum", sum);

		cv::waitKey(0);
	}

}
