/**
 ******************************************************************************
 * @file    types.cpp
 * @author  zlq
 * @version V1.0
 * @date    2016.9.14
 * @brief   ��Ҫ�Ǹ���ö�����͵Ķ���
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#ifndef _TYPES_C_H
#define _TYPES_C_H

enum
{
	BGR2GRAY = 0
};


enum {
	MORP_ERODE = 0,
	MORP_DILATE,
	MORP_OPEN,
	MORP_CLOSE,
	MORP_TOPHAT,
	MORP_BLACKHAT,
	MORP_GRADIENT
};
#endif // !_TYPES_C_H