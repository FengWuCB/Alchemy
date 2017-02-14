/**
 ******************************************************************************
 * @file    debug.h
 * @author  zlq
 * @version V1.0
 * @date    2016.9.17
 * @brief   ���Գ���ʹ��
 ******************************************************************************
 */
#include <Windows.h>
#include <iostream>
#include "black.h"
#include "config.h"

#ifndef _DEBUG_H
#define _DEBUG_H

/**
 * @berif ��ӡ��Ҫ�ĵ�����Ϣ
 */
#if defined(DEBUG)
#define _log_(str) st(std::cout<<"[" << __FILE__ << "]" << "<" << __func__ << ">" << "(" << __LINE__ << ") : " << str << std::endl;)
#else
#define _log_(str) 
#endif


/**
 * @berif ʱ���������һ�δ��������ʱ�䣬��λ��ms
 */
class TimeStamp {
public:
	/**
	 * @berif ���Կ�ʼ�ĵط�
	 */
	inline void start() { GetLocalTime(&starttime); }

	/**
	 * @berif ���Խ����ĵط�������ӡ��start()��ʼ���е�ʱ���
	 */
	inline int runtime()
	{
		GetLocalTime(&endtime);

		int diff = endtime.wMilliseconds - starttime.wMilliseconds;
		diff += (endtime.wSecond - starttime.wSecond) * 1000;
		diff += (endtime.wMinute - starttime.wMinute) * 60000;

        return  diff;
	}
private:
	SYSTEMTIME starttime, endtime;
};


#endif // !_DEBUG_H

