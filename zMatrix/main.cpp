#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include <ctime>  

#include "zcore.h"
#include "zimgproc\zimgproc.h"
#include "zimgproc\transform.h"
#include "zgui\zgui.h"

using namespace std;
using namespace z;


int main(int argc, char *argv[])
{
	Matrix8u gray, mdis, img = imread("test.jpeg");
	cv::Mat display, src = img;

	// zMatrix ��ȡ��ԭͼ
	cv::imshow("z org", cv::Mat(img));

	// zMatrix ��sobel���ӱ�Ե���Ч��
	cvtColor(img, gray, BGR2GRAY);
	sobel(gray, mdis, 1, 1);
	cv::imshow("z soble", cv::Mat(mdis));

	// openCV�е�sobel���ӱ�Ե���Ч��
	cv::Sobel(src, display, CV_8U, 1, 1);
	cv::imshow("cv sobel", display);

	// zMatrix �е�Canny��Ե���Ч��
	Canny(gray, mdis, 3, 6);
	cv::imshow("z Canny", cv::Mat(mdis));

	// openCV�е�Canny��Ե���Ч��
	cv::Canny(src, display, 30, 60);
	cv::imshow("cv Canny", display);

 	cv::waitKey(0);
	return 0;
}