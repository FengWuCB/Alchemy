#include <iostream>

#include"config_default.h"
#include "zmatrix.h"
#include "zimgproc.h"
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include "transform.h"

#include <ctime>  

using namespace std;
using namespace cv;
using namespace z;


int main(int argc, char *argv[])
{
	TimeStamp timestamp;
	Mat gray, display, src;
	src = imread("test.jpeg");
	Matrix8u mgray, mdis, mcolor = Mat2Matrix8u(src);

	// ԭͼ
	imshow("orginal", Mat(mcolor));

	// �Ҷ�ͼ
	z::cvtColor(mcolor, mgray, BGR2GRAY);
	imshow("gray", Mat(mgray));

	// canny ��ͼ��Ե���
	z::Canny(mcolor, mdis, 50, 100);
	imshow("canny color", Mat(mdis));

	// canny �Ҷ�ͼ��Ե���
	z::Canny(mgray, mdis, 50, 100);
	imshow("canny gray", Mat(mdis));

 	waitKey(0);
	return 0;
}