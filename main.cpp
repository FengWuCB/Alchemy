#include <iostream>
#include <opencv2\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <string>
#include <ctime>  

#include "zcore\zcore.h"
#include "zimgproc\zimgproc.h"
#include "zimgproc\transform.h"
#include "zgui\zgui.h"
#include "zcore\debug.h"

int main(int argc, char *argv[])
{
    // ����ԭʼͼ��
	z::Matrix8u test = z::imread("test.jpeg");
    cv::imshow("origin image", cv::Mat(test));

    // �Ҷ�ͼ
    z::Matrix8u gray(test.size(), 1);
    z::cvtColor(test, gray, BGR2GRAY);
    cv::imshow("gray", cv::Mat(gray));

    // ��ֵ�˲�
    z::medianFilter(gray, gray, z::Size(3, 3));

    // ��ֵ��
    auto bin_image = gray > 150;
    cv::imshow("binary image", cv::Mat(bin_image));

    // Ѱ������
    TimeStamp time;
    std::vector<std::vector<z::Point>> contours;
    time.start();
    z::findContours(bin_image, contours);
    time.runtime();
    cv::imshow("res", cv::Mat(bin_image));
    
    cv::waitKey(0);
	return 0;
}