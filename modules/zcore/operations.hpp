/**
 ******************************************************************************
 * @file    operations.hpp
 * @author  zlq
 * @version V1.0
 * @date    2016.9.7
 * @brief   ģ����_Matrix��ʵ��
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#ifndef _OPERATIONS_HPP
#define _OPERATIONS_HPP

#include "debug.h"

#ifdef __cplusplus

namespace z{

/**
 * @berif �������ʼ��Ϊ�վ���
 */
template <class _type>
void _Matrix<_type>::initEmpty()
{
	rows = cols = _size = 0;
	data = datastart = dataend = nullptr;
	refcount = nullptr;
	step = 0;
	chs = 0;
}

template<class _type>
inline void _Matrix<_type>::swap(int32_t i0, int32_t j0, int32_t i1, int32_t j1) {
	_type temp;
	for (uint8_t k = 0; k < chs; ++k) {
		temp = ptr(i0, j0)[k];
		ptr(i0, j0)[k] = ptr(i1, j1)[k];
		ptr(i1, j1)[k] = temp;
	}
}

/**
 * @berif �����Ĵ������󣬷����ڴ�
 * @attention ���о������ݵķ��䶼Ӧ��ͨ�����øú���ʵ�֣����øú���һ����ζ�����´���������
 * @param[in] _rows������
 * @param[in] _cols������
 */
template <class _type>
void _Matrix<_type>::create(int _rows, int _cols, int _chs)
{
	_log_("Matrix create.");

	chs = _chs;
	rows = _rows;
	cols = _cols;
	step = _cols * _chs;
	_size = _rows * _cols;

	// 
	release();

	// ����
	datastart = data = new _type[_rows * _cols * _chs];
	dataend = data + _size * _chs;
	refcount = new int(1);
}



/**
 * @berif Constructor without params.
 */
template <class _type> _Matrix<_type>::_Matrix()
{
	_log_("Matrix construct without params.");
	initEmpty();
}

template <class _type> _Matrix<_type>::_Matrix(_Size<int> size)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(size.width, size.height, 1);
}
template <class _type> _Matrix<_type>::_Matrix(_Size<int> size, int _chs)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(size.width, size.height, _chs);
}
/**
 * @berif Constructor with params.
 * @param[in] _rows������
 * @param[in] _cols������
 */
template <class _type> _Matrix<_type>::_Matrix(int _rows, int _cols)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(_rows, _cols, 1);
}

template <class _type> _Matrix<_type>::_Matrix(int _rows, int _cols, int _chs)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(_rows, _cols, _chs);
}

/**
 * @berif Copying function
 * @attention ����һ��ǳ����
 */
template <class _type> _Matrix<_type>::_Matrix(const _Matrix<_type>& m)
	:rows(m.rows), cols(m.cols), data(m.data), refcount(m.refcount),_size(m._size), 
	step(m.step),datastart(m.datastart), dataend(m.dataend), chs(m.chs)
{
	_log_("Matrix copying function.");
	if (refcount)
		refAdd(refcount, 1);
}

/**
 * @berif �������ü�����ֵ
 */
template <class _type>
int _Matrix<_type>::refAdd(int *addr, int delta)
{
	int temp = *addr;
	*addr += delta;
	return temp;
}

/**
 * @berif �ͷ���Դ
 * @attention �������Դ�ɸú������Ʋ��ͷ�
 */
template <class _type>
void _Matrix<_type>::release()
{
	if (refcount && refAdd(refcount, -1) == 1) {
		delete[] data;
		data = datastart = dataend = nullptr;
		delete refcount;
		refcount = nullptr;
		_log_("Matrix release.");
	}
}

/**
 * @berif Destructor
 */
template <class _type>
_Matrix<_type>::~_Matrix()
{
	release();
}


/**
 * @berif ����mat = {1, 2, 3}�ĸ�ֵ��ʽ
 */
template <class _type>
_Matrix<_type>& _Matrix<_type>::operator = (std::initializer_list<_type> li)
{
	if (rows == 0 || cols == 0) {
		create(1, li.size(), 1);
	}

	auto index = li.begin();
	auto end = li.end();
	for (_type * begin = datastart; begin < dataend; ++begin, ++index) {
		if (index < end) {
			*begin = *index;
		}
		else {
			*begin = (_type)0;
		}
	}
	return *this;
}

template <class _type> _Matrix<_type>& _Matrix<_type>::operator += (const _Matrix<_type>& m)
{
	for (size_t i = 0; datastart + i < dataend; ++i) {
		data[i] += m.data[i];
	}
	return (*this);
}
template <class _type> _Matrix<_type>&  _Matrix<_type>::operator -= (const _Matrix<_type>& m)
{
	for (size_t i = 0; datastart + i < dataend; ++i) {
		data[i] -= m.data[i];
	}
	return (*this);
}

/**
 * @berif �������ʼ��Ϊ0
 */
template <class _type>
void _Matrix<_type>::zeros()
{
	for (size_t i = 0; datastart +i < dataend; ++i) {
		data[i] = 0;
	}
}

/**
 * @berif ���·����ڴ沢��ʼ��Ϊ0
 */
template <class _type>
void _Matrix<_type>::zeros(int _rows, int _cols)
{
	create(_rows, _cols, 1);

	for (size_t i = 0; datastart + i < dataend; ++i) {
		data[i] = 0;
	}
}

/**
 * @berif �������ʼ��Ϊ1
 */
template <class _type>
void _Matrix<_type>::ones()
{
	for (size_t i = 0; datastart + i < dataend; ++i) {
		data[i] = 1;
	}
}

/**
 * @berif ���·����ڴ沢��ʼ��Ϊ1
 */
template <class _type>
void _Matrix<_type>::ones(int _rows, int _cols)
{
	create(_rows, _cols, 1);

	for (size_t i = 0; datastart + i < dataend; ++i) {
		data[i] = 1;
	}
}


/**
 * @berif �������ʼ��Ϊ��λ����
 */
template <class _type>
void _Matrix<_type>::eye()
{
	if (chs > 1)
		_log_("channels > 1!!");

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == j)
				data[i * cols + j] = 1;
			else
				data[i * cols + j] = 0;
		}
	}
}

/**
 * @berif ���·����ڴ沢��ʼ��Ϊ��λ����
 */
template <class _type>
void _Matrix<_type>::eye(int _rows, int _cols)
{
	create(_rows, _cols, 1);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if (i == j)
				data[i * cols + j] = 1;
			else
				data[i * cols + j] = 0;
		}
	}
}

/**
 * @berif ���������е�ֵ��ʼ��Ϊ_v
 */
template <class _type>
void _Matrix<_type>::init(_type _v)
{
	for (size_t i = 0; datastart + i < dataend; ++i)
		data[i] = _v;
}

/**
 * @berif ��ȸ��ƺ���
 * @param[out] outputMatrix�����Ƶ�Ŀ�ľ��󣬻ᱻ���·����ڴ沢��������
 */
template <class _type>
void _Matrix<_type>::copyTo(_Matrix<_type> & outputMatrix) const
{
	outputMatrix.create(rows, cols, chs);
	memcpy(outputMatrix.data, data, _size * chs * sizeof(_type));
}

/**
 * @berif ��ȸ��ƺ���
 * @ret ������ʱ����Ŀ���
 */
template <class _type>
_Matrix<_type> _Matrix<_type>::clone() const
{
	_Matrix<_type> m;
	copyTo(m);
	return m;
}
template <class _type> template <class _Tp2> _Matrix<_type>::operator _Matrix<_Tp2>() const
{
	_Matrix<_Tp2> temp(rows, cols, chs);
	const _type * ptr1 = 0;
	_Tp2 * ptr2 = 0;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols;++j) {

			ptr1 = this->ptr(i, j);
			ptr2 = temp.ptr(i, j);

			for (int k = 0; k < chs; ++k) {
				ptr2[k] = (_Tp2)ptr1[k];
			}
		}
	}
	return temp;
}


/**
 * @berif ��ֵ����
 * @attention ����һ��ǳ����
 */
template <class _type>
_Matrix<_type>& _Matrix<_type>::operator=(const _Matrix<_type> &m)
{
	_log_("Matrix assignment function.");
	// ��ֹ�����Լ����Լ�����ʱ�������
	if (this != &m) {
		if (m.refcount)
			refAdd(m.refcount, 1);

		// �ͷŵ���ֵ������
		release();

		// ��ֵ
		chs = m.chs;
		_size = m.size();
		data = m.data;
		refcount = m.refcount;
		rows = m.rows;
		cols = m.cols;
		step = m.step;
		datastart = m.datastart;
		dataend = m.dataend;
	}

	return *this;
}


template <class _type>
_Matrix<_type>& _Matrix<_type>::operator()(_type * InputArray, size_t _size)
{
	create(1, _size, 1);
	for (size_t i = 0; i < _size; ++i)
		data[i] = InputArray[i];

	return *this;
}

template <class _type>
_Matrix<_type>& _Matrix<_type>::operator()(_type * InputArray, int _rows, int _cols)
{
	create(_rows, _cols, 1);
	for (size_t i = 0; datastart + i < dataend; ++i)
		data[i] = InputArray[i];

	return *this;
}

#if defined(OPENCV)
/**
 * @berif ��openCV�е�Mat��ת��
 */
template <class _type>
_Matrix<_type>::operator cv::Mat() const
{
	cv::Mat temp(rows, cols, CV_8UC(chs));

	memcpy(temp.data, data, _size * chs * sizeof(_type));

	return temp;
}
#endif
template <class _type> inline _type* _Matrix<_type>::ptr(int i0)
{
	if ((unsigned)i0 >= (unsigned)_size) {
		return nullptr;
	}
	return data + i0 * step;
}


template <class _type> inline const _type* _Matrix<_type>::ptr(int i0) const
{
	if ((unsigned)i0 >= (unsigned)_size) {
		return nullptr;
	}
	return data + i0 * step;
}
/**
 * @berif ��ȡ�����ص�ָ��
 */
template <class _type> inline _type* _Matrix<_type>::ptr(int i0, int i1)
{
	if ( (unsigned)i0 >= (unsigned)rows || (unsigned)i1 >= (unsigned)cols) {
		return nullptr;
	}
	return data + i0 * step + i1 * chs;
}
/**
 * @berif ��ȡ�����ص�ָ��
 */
template <class _type> inline const _type* _Matrix<_type>::ptr(int i0, int i1) const
{
	if ((unsigned)i0 >= (unsigned)rows || (unsigned)i1 >= (unsigned)cols) {
		return nullptr;
	}
	return data + i0 * step + i1 * chs;
}

/**
 * @berif ��������
 * m x n������min(m, n)�������
 */
template <class _type>
_type _Matrix<_type>::rank()
{
	_type temp = (_type)0;
	// do something..
	return temp;
}

/**
 * @berif �����ļ������Խ���Ԫ��֮��
 * @attention 1����������Ƿ���
 *            2�����ڼ��ǶԽ���Ԫ��֮�ͣ����Զ���char��short�ȿ��ܻᷢ�����������ͬһ��Ϊdouble
 * m x n������min(m, n)�������
 */
template <class _type>
double _Matrix<_type>::tr()
{
	if (chs != 1)
		_log_("chs != 1");
	if (rows != cols)
		_log_("rows != cols");

	_type temp = (_type)0;
	for (int i = 0; i < rows; ++i) {
		temp += (*this)[i][i];
	}
	return temp;
}


/**
 * @berif ��
 */
template <class _type>
_Matrix<_type> _Matrix<_type>::inv()
{
	_Matrix<_type> m(cols, rows);
	// do something..
	return m;
}


/**
 * @berif ת��
 */
template <class _type>
_Matrix<_type>  _Matrix<_type>::t()
{
	_Matrix<_type> m(cols, rows, chs);

	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			for (int k = 0; k < chs; ++k) {
				m[i][j * chs + k] = (*this)[j][i * chs + k];
			}
		}
	}
	return m;
}

/**
 * @berif ���
 */
template <class _type>
_Matrix<_type> _Matrix<_type>::dot(_Matrix<_type> &m)
{
	if (rows != m.rows || cols != m.cols || chs != chs)
		_log_("rows != m.rows || cols != m.cols || || chs != chs");

	_Matrix<_type> temp(m.rows, m.cols, m.chs);

	for (size_t i = 0; datastart + i < dataend; ++i) {
		temp.data[i] = data[i] * m.data[i];
	}

	return temp;
}

/**
 * @berif ���
 * @attention C = cross(A,B) returns the cross product of the vectors
 *            A and B.  That is, C = A x B.  A and B must be 3 element
 *            vectors.
 */
template <class _type>
_Matrix<_type> _Matrix<_type>::cross(_Matrix<_type> &m)
{
	if (rows != 1 || cols != 3 || m.rows != 1 || m.cols != 3 || chs != 0)
		_log_("rows != 1 || cols != 3 || m.rows != 1 || m.cols != 3 || chs != 0");

	_Matrix<_type> temp(1, 3);

	temp[0][0] = data[1] * m.data[2] - data[2] * m.data[1];
	temp[0][1] = data[2] * m.data[0] - data[0] * m.data[2];
	temp[0][2] = data[0] * m.data[1] - data[1] * m.data[0];

	return temp;
}

/**
 * @berif �������
 *
 * @param[in] kernel�� �����
 * @param[out] dst�����
 * @param[in] norm�� �Ƿ�Ծ�������һ��
 */
template <class _type> void _Matrix<_type>::conv(Matrix &kernel, _Matrix<_type>&dst, bool norm)
{
	if (kernel.rows != kernel.cols || kernel.rows % 2 == 0)
		_log_("size.width != size.height || size.width % 2 == 0");

	if (!dst.equalSize(*this))
		dst.create(rows, cols, chs);

	int *tempValue = new int[chs];                            // ������δ��һ��ǰ���м����
	int zeros = 0;                                            // ��Ե������¼��Ե��������еĳ���ͼ������ظ���
	int m = kernel.rows / 2, n = kernel.cols / 2;
	const _type * srcPtr = nullptr;                           // ָ��Դͼ�����ص�ָ��
	_type * dstPtr = nullptr;                                 // ָ�����ͼ������ص�ָ��
	int alpha = 0;
	int delta = 0;
	for (size_t i = 0; i < kernel.size(); ++i) {
		delta += (int)kernel.data[i];
	}

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {

			memset(tempValue, 0, chs * sizeof(int));
			zeros = 0;

			for (int ii = 0; ii < kernel.rows; ++ii) {
				for (int jj = 0; jj < kernel.cols; ++jj) {

					// ��ȡһ�����صĵ�ַ
					srcPtr = this->ptr(i - m + ii, j - n + jj);

					if (srcPtr) {
						for (int k = 0; k < chs; ++k) {
							tempValue[k] += srcPtr[k] * kernel[ii][jj];
						}
					}
					else {
						zeros++;
					}
				} // !for(jj)
			} // !for(ii)

			alpha = delta - zeros;
			dstPtr = dst.ptr(i, j);

			for (int k = 0; k < chs; ++k) {
				if(norm)
					dstPtr[k] = (_type)(tempValue[k] / alpha);
				else
					dstPtr[k] = (_type)tempValue[k];
			}

		} // !for(j)
	} // !for(i)

	delete[] tempValue;
}

template <class _type> void conv(_Matrix<_type> &src, _Matrix<_type> &dst, Matrix &core)
{
	src.conv(core, dst);
}
/**
 * @berif ������������
 */
template <class _type>
std::ostream &operator<<(std::ostream & os, const _Matrix<_type> &item)
{
	os << '[';
	for (int i = 0; i < item.rows; ++i) {
		for (int j = 0; j < item.step; ++j) {
			
			if(sizeof(_type) == 1)
				os << (int)item[i][j];
			else
				os << item[i][j];
			if (item.cols * item.chs != j + 1)
				os << ", ";
		}
		if (item.rows != i + 1)
			os << ';' << std::endl << ' ';
		else
			os << ']' << std::endl;
	}
	return os;
}

/**
 * @berif �Ƚ����������Ƿ����
 */
template <class _type>
bool operator==(const _Matrix<_type> &m1, const _Matrix<_type> &m2)
{
	// 1��û�з����ڴ�ľ���Ƚϣ�û�о���create()
	if (m1.data == nullptr && m1.data == m2.data) {
		return true;
	}
	// 2�������ڴ�
	else if (m1.data != nullptr) {
		// �ڴ��ַ��ȣ����ã����
		if (m1.data == m2.data)
			return true;
		// ��ַ�����, ������ȵ�ǰ���£�Ԫ�����
		else {
			if (m1.cols == m2.cols && m1.rows == m2.rows && m1.chs == m2.chs) {
				int i = 0;
				for (; m1.datastart + i < m1.dataend; ++i) {
					if (m1.data[i] != m2.data[i])
						break;
				}
				if (i == m1.size())
					return true;
			}
		}
	}
	return false;
}

/**
 * @berif �Ƚ����������Ƿ����
 */
template <class _type>
bool operator!=(const _Matrix<_type> &m1, const _Matrix<_type> &m2)
{
	return !(m1 == m2);
}

/**
 * @berif ����˷�������*
 */
template <class _type>
_Matrix<_type> operator*(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	if (m1.chs != 1 || m2.chs != 1)
		_log_("m1.chs != 1 || m2.chs != 1");
	if (m1.cols != m2.rows)
		_log_("m1.cols != m2.rows");

	_Matrix<_type> m(m1.rows, m2.cols, 1);
	m.zeros();

	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			for (int k = 0; k < m1.cols; ++k) {
				m[i][j] += m1[i][k] * m2[k][j];
			}
		}
	}

	return m;
}

/**
 * @berif ����ӷ�������+
 */
template <class _type>
_Matrix<_type> operator+(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	if (m1.cols != m2.cols || m1.rows != m2.rows)
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");

	_Matrix<_type> temp(m1.rows, m1.cols, m1.chs);

	for (size_t i = 0; m1.datastart + i < m1.dataend; ++i) {
		temp.data[i] = m1.data[i] + m2.data[i];
	}
	return temp;
}

/**
 * @berif �������������-
 */
template <class _type>
_Matrix<_type> operator-(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	if (m1.cols != m2.cols || m1.rows != m2.rows)
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");

	_Matrix<_type> temp(m1.rows, m1.cols, m1.chs);

	for (size_t i = 0; m1.datastart + i < m1.dataend; ++i) {
		temp.data[i] = m1.data[i] - m2.data[i];
	}
	return temp;
}

/**
 * @berif �������ˣ�����*
 */
template <class _type>
_Matrix<_type> operator*(_Matrix<_type> &m, _type delta)
{
	_Matrix<_type> temp(m.rows, m.cols, m.chs);

	for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
		temp.data[i] = m.data[i] * delta;
	}

	return temp;
}

template <class _type>
_Matrix<_type> operator*(_type delta, _Matrix<_type> &m)
{
	return m*delta;
}

/**
 * @berif ����ӷ�������+
 */
template <class _type>
_Matrix<_type> operator+(_Matrix<_type> &m, _type delta)
{
	_Matrix<_type> temp(m.rows, m.cols, m.chs);

	for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
		temp.data[i] = m.data[i] + delta;
	}

	return temp;
}
template <class _type>
_Matrix<_type> operator+(_type delta, _Matrix<_type> &m)
{
	return m + delta;
}

/**
 * @berif �������������-
 */
template <class _type>
_Matrix<_type> operator-(_Matrix<_type> &m, _type delta)
{
	return m + (-delta);
}
template <class _type>
_Matrix<_type> operator-(_type delta, _Matrix<_type> &m)
{
	return m * (-1) + delta;
}

template <class _T1, class _T2> _Matrix<_T1> operator>(_Matrix<_T1> &m, _T2 threshold)
{
    _Matrix<_T1> temp(m.rows, m.cols, m.chs);
    temp.zeros();

    for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
        if (m.data[i] > threshold)
            temp.data[i] = 255;
    }

    return temp;
}


template <class _T1, class _T2> _Matrix<_T1> operator<(_Matrix<_T1> &m, _T2 threshold)
{
    _Matrix<_T1> temp(m.rows, m.cols, m.chs);
    temp.zeros();

    for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
        if (m.data[i] < threshold)
            temp.data[i] = 255;
    }

    return temp;
}

template <class _T1, class _T2> _Matrix<_T1> operator>=(_Matrix<_T1> &m, _T2 threshold)
{
    _Matrix<_T1> temp(m.rows, m.cols, m.chs);
    temp.zeros();

    for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
        if (m.data[i] >= threshold)
            temp.data[i] = 255;
    }

    return temp;
}

template <class _T1, class _T2> _Matrix<_T1> operator<=(_Matrix<_T1> &m, _T2 threshold)
{
    _Matrix<_T1> temp(m.rows, m.cols, m.chs);
    temp.zeros();

    for (size_t i = 0; m.datastart + i < m.dataend; ++i) {
        if (m.data[i] <= threshold)
            temp.data[i] = 255;
    }

    return temp;
}

/////////////////////////////////////////_Complex_////////////////////////////////////////////
template <class _Tp> inline _Complex_<_Tp>::_Complex_() :re(0), im(0) {}
template <class _Tp> inline _Complex_<_Tp>::_Complex_(_Tp _re, _Tp _im) : re(_re), im(_im) {}
template <class _Tp> inline _Complex_<_Tp>::_Complex_(const _Complex_&c) : re(c.re), im(c.im) {}
template <class _Tp> _Complex_<_Tp> & _Complex_<_Tp>::operator=(const _Complex_ &c) { re = c.re; im = c.im; return *this; };

template <class _Tp> bool operator ==(const _Complex_<_Tp> & c1, const _Complex_<_Tp> &c2)
{
	return (c1.re == c2.re && c1.im == c2.im);
}
template <class _Tp> bool operator !=(const _Complex_<_Tp> & c1, const _Complex_<_Tp> &c2)
{
	return !(c1 == c2);
}

template <class _Tp> _Complex_<_Tp> operator*(const _Complex_<_Tp> & c1, const _Complex_<_Tp> & c2)
{
	return _Complex_<_Tp>(c1.re * c2.re - c1.im * c2.im, c1.im * c2.re + c1.re * c2.im);
}

template <class _Tp> _Complex_<_Tp> operator+(const _Complex_<_Tp> & c1, const _Complex_<_Tp> &c2)
{
	return _Complex_<_Tp>(c1.re + c2.re, c1.im + c2.im);
}

template <class _Tp> _Complex_<_Tp> operator-(const _Complex_<_Tp> & c1, const _Complex_<_Tp> &c2)
{
	return _Complex_<_Tp>(c1.re - c2.re, c1.im - c2.im);
}

template <class _Tp> _Complex_<_Tp>& _Complex_<_Tp>::operator+=(const _Complex_<_Tp> & c)
{
	re += c.re;
	im += c.im;
	return *this;
}

template <class _Tp> _Complex_<_Tp>& _Complex_<_Tp>::operator-=(const _Complex_<_Tp> & c)
{
	re -= c.re;
	im -= c.im;
	return *this;
}

template <class _Tp> std::ostream & operator<<(std::ostream & os, const _Complex_<_Tp> & c)
{
	os << "(" << c.re << ", " << c.im << ")";
}

}

#endif // ! __cplusplus

#endif