/**
 ******************************************************************************
 * @file    transform.h
 * @author  zlq
 * @version V1.0
 * @date    2016.9.14
 * @brief   ͼ��任�ĺ�������
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#ifndef _TRANSFORM_H
#define _TRANSFORM_H


#include "zcore\zmatrix.h"
#include "zmatch\zmatch.h"
#include "zimgproc.h"

#define RAD2ANG			((double)(57.296))

typedef enum {
    OUTER_BORDERS,
    ALL_BORDERS,
}ContourType;

#ifdef __cplusplus
namespace z {
// ͼ�񼸺α任
// ƽ��
void translation(Matrix8u &src, Matrix32s &kernel, Matrix8u &dst);

///////////////////////////////////////////////Edge Detector/////////////////////////////////////////////////
void getSobelKernels(Matrix8s &kx, Matrix8s &ky, int dx, int dy, int ksize);
void prewitt(Matrix8u&src, Matrix8u&dst);
void sobel(Matrix8u&src, Matrix8u&dst, int dx = 1, int dy = 1, int ksize = 3);
void Canny(Matrix8u&src, Matrix8u&dst, double threshold1, double threshold2, int apertureSize = 3);

///////////////////////////////////////////////Contours/////////////////////////////////////////////////
/**
 * @berif ��ȡ��������
 * @attention �ú�����ı���������ֵ
 * @param[in/out] src ����Ķ�ֵͼ��
 * @param[out] dst, �߽�㼯������
 * @ret None
 */
void findContours(Matrix8u &src, std::vector<std::vector<Point>> &dst);

/**
 * @berif ��ȡ��������
 *      ��� [ 0 | 255 ] Ϊ 2��[ 255 | 0 ]��Ϊ 254(-2)��
 *      ���ɨ���2��˵���ڱ߽��ڣ�����-2˵���ڱ߽���
 * @attention �ú�����ı���������ֵ
 * @param[in/out] src ����Ķ�ֵͼ��
 * @param[out] dst, �߽�㼯������
 * @ret None
 */
void findOutermostContours(Matrix8u &src, std::vector<std::vector<Point>> &dst);
}


#endif

#include "transform.hpp"

#endif // !_TRANSFORM_H
