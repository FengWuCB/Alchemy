#ifndef _TEST_H
#define _TEST_H

#include "zcore\black.h"

#define ASSERT_FALSE(condition) 
#define ASSERT_TRUE(condition)

#define ASSERT_EQ(expected,actual)
#define ASSERT_NE(expected,actual)


bool test_all();

/*-------------------zMatrix��Ĳ���-------------------*/
bool test_zmatrix();

/*-------------------jpeg�ļ���д-------------------*/
bool test_imread();
bool test_imwrite();

/*-------------------�����˲�����-------------------*/
bool test_Gassion();
bool test_blur();
bool test_boxFilter();
bool test_GaussianBlur();
/*-------------------�������˲�����-------------------*/
bool test_medianFilter();

/*-------------------��̬ѧ�˲�����-------------------*/
bool test_morphOp();
bool test_erode();
bool test_dilate();

bool test_morphEx();
bool test_open();

/*-------------------ͼ��任-------------------*/

#endif // !_TEST_H