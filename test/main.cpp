#include <iostream>
#include <opencv2\opencv.hpp>
#include <fstream>

#include "zmatrix.h"

int main(int argc, char *argv[])
{
    auto test = z::imread("test.jpeg");
    z::Matrix gray;
    
    // �Ҷ�ͼ
    z::cvtColor(test, gray, BGR2GRAY);
    z::Matrix median;
    // �˲�
    z::medianFilter(gray, median, { 7, 7 });

    z::SIFT detector;
    detector.detect(z::Matrix64f(median));

    cv::waitKey(0);

    return 0;
}