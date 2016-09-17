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
#include "debug.h"

using namespace std;
using namespace z;


int main(int argc, char *argv[])
{
	Matrix8u zcolor, zgray, zdis;
	cv::Mat cvColor, cvGray, cvdis;
	TimeStamp timestamp;

	// ��ȡһ��ͼƬ
	zcolor = imread("test.jpeg");

	// ��ʾͼƬ��Ҫ��openCV�ĺ���
	cv::imshow("zcolor", cv::Mat(zcolor));

	// ת��Ϊ�Ҷ�ͼ��
	cvtColor(zcolor, zgray, BGR2GRAY);
	cv::imshow("zgray", cv::Mat(zgray));

	// ��˹�˲�
	GaussianBlur(zcolor, zdis, Size(5, 5));
	cv::imshow("z GassianBlar", cv::Mat(zdis));

	// sobel һ��΢�����ӱ�Ե���
	sobel(zgray, zdis, 1, 1, 3);
	cv::imshow("z sobel", cv::Mat(zdis));

 	cv::waitKey(0);
	return 0;
}