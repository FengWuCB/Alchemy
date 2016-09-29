/**
 ******************************************************************************
 * @file    zmatch.cpp
 * @author  zlq
 * @version V1.0
 * @date    2016.9.14
 * @brief   �򵥵���ѧ��������
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#ifndef _ZMATCH_H
#define _ZMATCH_H


#define Pi			((double)3.1415926535898)


template <typename _Tp> void _min(_Tp *addr, size_t size, _Tp & _min);
template <typename _Tp> void _max(_Tp *addr, size_t size, _Tp & _max);

int average(int x, int y);


/**
 * @berif �������е������
 * @param size������Ĵ�С or ��Ҫ�Ƚ�����ǰsize��
 */
template <typename _Tp> void _max(_Tp *addr, size_t size, _Tp & _max)
{
	_max = *addr;
	_Tp * begin = addr + 1;
	_Tp *end = addr + size;

	for (; begin < end; ++begin) {
		if (_max < begin[0])
			_max = begin[0];
	}
}


/**
 * @berif �������е������
 * @param size������Ĵ�С or ��Ҫ�Ƚ�����ǰsize��
 */
template <typename _Tp> void _min(_Tp *addr, size_t size, _Tp & _min)
{
	_min = *addr;
	_Tp * begin = addr + 1;
	_Tp *end = addr + size;

	for (; begin < end; ++begin) {
		if (_min > begin[0])
			_min = begin[0];
	}
}



#endif // !_ZMATCH_H
