#include "zmath.h"


/**
 * @brief �����������ľ�ֵ
 * @attention ��ֹ(x + y) / 2�������
 */
int average(int x, int y)
{
	return (x & y) + ((x ^ y) >> 1);
}
