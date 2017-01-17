/**
 ******************************************************************************
 * @file    transform.cpp
 * @author  zlq
 * @version V1.0
 * @date    2016.9.14
 * @brief   ͼ��任�ĺ���ʵ��
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#include <cmath>
#include "transform.h"
#include "zcore\debug.h"

namespace z {

/**
 * @declaration ��������
 *              �ڲ�����
 */ 
static inline void only_max(Matrix8u&src, Matrix8u&dst, Matrix8u&srcGD);
static void double_threashold(Matrix8u&src, Matrix8u&dst, double threshold1, double threshold2);
static void sobel(Matrix8u&src, Matrix8u&dst, Matrix8u&dstGD, int dx = 1, int dy = 1, int ksize = 3, bool noGD = false);

/**
 * @berif Canny�еķǼ���ֵ����
 */
inline void only_max(Matrix8u&src, Matrix8u&dst, Matrix8u&srcGD)
{
	unsigned char * srcptr, * srcptr1, *dstptr;

	unsigned char ang;

	if (!src.equalSize(srcGD))
		throw std::runtime_error("src.equalSize(srcGD)!");

	dst = src.clone();
	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			
			srcptr = src.ptr(i, j);
			dstptr = dst.ptr(i, j);

			for (int k = 0; k < src.chs; k++) {

				ang = srcGD.ptr(i, j)[k];

				if (ang == 0) {

					srcptr1 = src.ptr(i, j - 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
					srcptr1 = src.ptr(i, j + 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
				}
				else if (ang == 45) {
					srcptr1 = src.ptr(i - 1, j + 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
					srcptr1 = src.ptr(i + 1, j - 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
				}
				else if (ang == 90) {
					srcptr1 = src.ptr(i - 1, j);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
					srcptr1 = src.ptr(i + 1, j);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
				}
				else if (ang == 135) {
					srcptr1 = src.ptr(i - 1, j - 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
					srcptr1 = src.ptr(i + 1, j + 1);
					if (srcptr1 != 0) {
						if (srcptr[k] < srcptr1[k]) {
							dstptr[k] = 0;
						}
					}
				}
				

			} // for(k)
		} // for(j)
	} //  for(i)
}


/**
 * @berif Canny�е�˫��ֵ
 */
void double_threashold(Matrix8u&src, Matrix8u&dst, double threshold1, double threshold2)
{
	double maxt = threshold1 > threshold2 ? threshold1 : threshold2;
	double mint = threshold1 < threshold2 ? threshold1 : threshold2;

	if (!dst.equalSize(src))
		dst.create(src.rows, src.cols, src.chs);

	unsigned char *ptr, * ptr1, *ptr2, *ptr3, *ptr4, *ptr5, *ptr6, *ptr7, *ptr8, * dstPtr;

	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			ptr = src.ptr(i, j);

			ptr1 = src.ptr(i - 1, j);             // ��
			ptr2 = src.ptr(i, j - 1);             // ��
			ptr3 = src.ptr(i, j + 1);             // ��
			ptr4 = src.ptr(i + 1, j);             // ��
			ptr5 = src.ptr(i - 1, j - 1);         // ����     
			ptr6 = src.ptr(i - 1, j + 1);         // ����
			ptr7 = src.ptr(i + 1, j + 1);         // ����
			ptr8 = src.ptr(i + 1, j - 1);         // ����

			dstPtr = dst.ptr(i, j);

			for (int k = 0; k < src.chs; ++k) {

				if (ptr[k] < mint) {
					dstPtr[k] = 0;
				}
				else if (ptr[k] > maxt) {
					dstPtr[k] = 255;
				}
				else if ((ptr1 != 0 && ptr1[k] > maxt)
					|| (ptr2 != 0 && ptr2[k] > maxt)
					|| (ptr3 != 0 && ptr3[k] > maxt)
					|| (ptr4 != 0 && ptr4[k] > maxt)
					|| (ptr5 != 0 && ptr5[k] > maxt)
					|| (ptr6 != 0 && ptr6[k] > maxt)
					|| (ptr7 != 0 && ptr7[k] > maxt)
					|| (ptr8 != 0 && ptr8[k] > maxt)) {
					dstPtr[k] = 255;
				}
				else {
					dstPtr[k] = 0;
				}

			}
		}
	}
}

//////////////////////////////////////һ��΢������///////////////////////////////////////////
// ��Ҫ���⣺�õ��ı�Ե��Ҷȹ��ȷ�Χ�ȿ���˱�Ե�����޷�����ȷ��λ
//////////////////////////////////////һ��΢������///////////////////////////////////////////

/**
 * @berif prewitt����
 */
void prewitt(Matrix8u&src, Matrix8u&dst)
{
	if (!dst.equalSize(src))
		dst.create(src.rows, src.cols, src.chs);

	Matrix8s Gx(3, 3), Gy(3, 3);

	Gx = { -1, 0, 1, -1, 0, 1, -1, 0, 1 };
	Gy = { -1, -1, -1, 0, 0, 0, 1, 1, 1 };

	int factor = 6;

	int *tempGx = new int[src.chs];
	int *tempGy = new int[src.chs];
	int *tempG = new int[src.chs];
	int zerosx = 0, zerosy = 0;
	unsigned char * srcPtr = nullptr;
	unsigned char * dstPtr = nullptr;

	for (int i = 0; i < dst.rows; ++i) {
		for (int j = 0; j < dst.cols; ++j) {

			memset(tempGx, 0, src.chs * sizeof(int));
			memset(tempGy, 0, src.chs * sizeof(int));
			memset(tempG, 0, src.chs * sizeof(int));
			zerosx = zerosy = 0;

			for (int ii = 0; ii < 3; ++ii) {
				for (int jj = 0; jj < 3; ++jj) {

					srcPtr = src.ptr(i - 1 + ii, j - 1 + jj);

					if (srcPtr) {
						for (int k = 0; k < src.chs; ++k) {
							tempGx[k] += srcPtr[k] * Gx[ii][jj];
							tempGy[k] += srcPtr[k] * Gy[ii][jj];
						}
					}
					else {
						zerosx += Gx[ii][jj];
						zerosy += Gy[ii][jj];
					}

				} // !for(jj)
			} // !for(ii)

			  // �ֲ��ݶȷ����ĵĹ��ƣ�ͨ�����˲���������ʵ��ĳ߶�������ʵ��
			for (int k = 0; k < src.chs; ++k) {
				if (zerosx != 0) {
					tempGx[k] /= zerosx;
				}
				else {
					tempGx[k] /= factor;
				}

				if (zerosy != 0) {
					tempGy[k] /= zerosy;
				}
				else {
					tempGy[k] /= factor;
				}
			}

			dstPtr = dst.ptr(i, j);
			for (int k = 0; k < src.chs; ++k) {
				dstPtr[k] = (unsigned char)std::sqrt(tempGx[k] * tempGx[k] + tempGy[k] * tempGy[k]);
			}


		} // !for(j)
	} // !for(i)

	delete[] tempGx;
	delete[] tempGy;
	delete[] tempG;
}

/**
 * @berif sobel����
 * @param[in] ksize, must be 1, 3, 5 or 7.
 * @param[in] dx
 * @param[in] dy
 * @ksize[in] ����˵Ĵ�С
 */
void sobel(Matrix8u&src, Matrix8u&dst, int dx, int dy, int ksize)
{
	Matrix8u temp;
	sobel(src, dst, temp, dx, dy, ksize, true);
}

/**
 * @berif sobel����
 * @param[in] src
 * @param[out] dst
 * @param[out] dstGD��
 * @param[in] ksize, must be 1, 3, 5 or 7.
 * @param[in] dx
 * @param[in] dy
 * @ksize[in] ����˵Ĵ�С
 * @param[in] noGD���Ƿ�����ݶȷǼ���ֵ����
 */
void sobel(Matrix8u&src, Matrix8u&dst, Matrix8u&dstGD, int dx, int dy, int ksize, bool noGD)
{
	if (!src.equalSize(dst))
		dst.create(src.rows, src.cols, src.chs);
	if (!noGD)
		if (!dstGD.equalSize(src))
			dstGD.create(src.rows, src.cols, src.chs);

	Matrix8s Gx(ksize, ksize), Gy(ksize, ksize);

	int factor = 0;

	switch (ksize) {
	case 1:
		
		break;

	case 3:
		// ԭʼsobel����
		//Gx = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
		//Gy = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
		//factor = 8;
		// �Ľ��ͣ����Խ�������������С
		Gx = { -3, 0, 3, -10, 0, 10, -3, 0, 3 };
		Gy = { -3, -10, -3, 0, 0, 0, 3, 10, 3 };
		factor = 32;
		break;

	case 5:
		break;

	case 7:
		break;

	default:
		_log_("Error ksize!");
		return;
	}
	

	int *tempGx = new int[src.chs];
	int *tempGy = new int[src.chs];
	int *tempG = new int[src.chs];
	int zerosx = 0, zerosy = 0;
	int m = ksize / 2, n = ksize / 2;
	unsigned char * srcPtr = nullptr;
	unsigned char * dstPtr = nullptr;
	unsigned char * dstGDPtr = nullptr;
	int alpha = 0;
	double ang = 0;

	for (int i = 0; i < dst.rows; ++i) {
		for (int j = 0; j < dst.cols; ++j) {

			memset(tempGx, 0, src.chs * sizeof(int));
			memset(tempGy, 0, src.chs * sizeof(int));
			memset(tempG, 0, src.chs * sizeof(int));
			zerosx = zerosy = 0;

			for (int ii = 0; ii < ksize; ++ii) {
				for (int jj = 0; jj < ksize; ++jj) {

					srcPtr = src.ptr(i - m + ii, j - n + jj);

					if (srcPtr) {
						for (int k = 0; k < src.chs; ++k) {
							tempGx[k] += srcPtr[k] * Gx[ii][jj];
							tempGy[k] += srcPtr[k] * Gy[ii][jj];
						}
					}
					else {
						zerosx += Gx[ii][jj];
						zerosy += Gy[ii][jj];
					}

				} // !for(jj)
			} // !for(ii)

			// �ֲ��ݶȷ����ĵĹ��ƣ�ͨ�����˲���������ʵ��ĳ߶�������ʵ��
			for (int k = 0; k < src.chs; ++k) {
				if (zerosx != 0) {
					tempGx[k] /= zerosx;
				}
				else {
					tempGx[k] /= factor;
				}
				
				if (zerosy != 0) {
					tempGy[k] /= zerosy;
				}
				else {
					tempGy[k] /= factor;
				}
			}
			

			dstPtr = dst.ptr(i, j);
			if (!noGD)
				dstGDPtr = dstGD.ptr(i, j);

			for (int k = 0; k < src.chs; ++k) {
				dstPtr[k] = (unsigned char)std::sqrt(tempGx[k] * tempGx[k] + tempGy[k] * tempGy[k]);
				// �����ݶ�
				if (!noGD) {
					ang = atan2(tempGy[k],tempGx[k]) * RAD2ANG;

					if ((ang > -22.5 && ang < 22.5) || (ang > 157.5 || ang < -157.5))
						dstGDPtr[k] = 0;
					else if ((ang > 22.5 && ang < 67.5) || (ang < -112.5 && ang > -157.5))
						dstGDPtr[k] = 45;
					else if ((ang > 67.5 && ang < 112.5) || (ang < -67.5 && ang > -112.5))
						dstGDPtr[k] = 90;
					else if ((ang < -22.5 && ang > -67.5) || (ang > 112.5 && ang  < 157.5))
						dstGDPtr[k] = 135;
				}
			}


		} // !for(j)
	} // !for(i)

	delete[] tempGx;
	delete[] tempGy;
	delete[] tempG;
}




/**
 * @berif Canny ��Ե����㷨
 *
 * @param[in] src����Ҫ�����ͼ��
 * @param[out] dst�����ͼ��
 * @param[in] threshold1��˫��ֵ��һ����ֵ
 * @param[in] threshold2��˫��ֵ�ڶ�����ֵ
 */
void Canny(Matrix8u&src, Matrix8u&dst, double threshold1, double threshold2, int apertureSize)
{
	// �м����
	Matrix8u dstGD;
	Matrix8u temp, temp1, temp2;

	// ��һ������˹�˲�
	GaussianBlur(src, temp, z::Size(5, 5));

	// �ڶ�����ʹ��sobel����
	sobel(temp, temp1, dstGD, 1, 1, apertureSize);

	// ������,�Ǽ���ֵ����
	only_max(temp1, temp2, dstGD);
	
	// ���Ĳ���˫��ֵ
	double_threashold(temp2, dst, threshold1, threshold2);
}

void remap(Matrix8u &src, Matrix32s &kernel, Matrix8u &dst)
{
	if (!dst.equalSize(src)) {
		dst.create(src.rows, src.cols, src.chs);
	}
	dst.zeros();

	Matrix32s srcCoord(1,3,1), dstCoord;

	for (int i = 0; i < src.rows; ++i) {
		for (int j = 0; j < src.cols; ++j) {
			srcCoord = { i, j, 1 };
			dstCoord = srcCoord * kernel;

			for (int k = 0; k < src.chs && (dstCoord[0][0] < dst.rows && dstCoord[0][1] < dst.cols &&  dstCoord[0][0] > 0 && dstCoord[0][1] > 0); ++k) {
				dst.ptr(dstCoord[0][0], dstCoord[0][1])[k] = src.ptr(i, j)[k];
			}
		
		}
	}
}


} // ! namespace z

