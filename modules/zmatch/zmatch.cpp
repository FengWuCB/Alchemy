#include "zmatch.h"


/**
 * @berif �����������ľ�ֵ
 * @ �ڿƼ�����ֹ(x + y) / 2�������
 */
int average(int x, int y)
{
	return (x & y) + ((x ^ y) >> 1);
}
