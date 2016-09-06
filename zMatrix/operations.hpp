#ifndef _OPERATION_HPP
#define _OPERATION_HPP

#ifdef __cplusplus

/**
* @berif �������ʼ��Ϊ�վ���
*/
template <class _type>
void _Matrix<_type>::initEmpty()
{
	rows = cols = _size = 0;
	data = nullptr;
	refcount = nullptr;
}

/**
* @berif �����Ĵ������󣬷����ڴ�
* @attention ���о������ݵķ��䶼Ӧ��ͨ�����øú���ʵ�֣����øú���һ����ζ�����´���������
* @param[in] _rows������
* @param[in] _cols������
*/
template <class _type>
void _Matrix<_type>::create(int _rows, int _cols)
{
	_log_("Matrix create.");

	rows = _rows;
	cols = _cols;
	_size = rows * cols;

	// 
	release();

	// ����
	data = new _type[_rows * _cols];
	refcount = new int(1);
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
		data = nullptr;
		delete refcount;
		refcount = nullptr;
		_log_("Matrix release.");
	}
}

/**
* @berif �޲ι��캯��
*/
template <class _type> _Matrix<_type>::_Matrix()
{
	_log_("Matrix construct without params.");
	initEmpty();
}

/**
* @berif ���캯��
* @param[in] _rows������
* @param[in] _cols������
*/
template <class _type> _Matrix<_type>::_Matrix(int _rows, int _cols)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(_rows, _cols);
}
/**
* @berif ��������
* @attention ����һ��ǳ����
*/
template <class _type> _Matrix<_type>::_Matrix(const _Matrix<_type>& m)
	:rows(m.rows), cols(m.cols), data(m.data), refcount(m.refcount)
{
	_log_("Matrix copying function.");
	if (refcount)
		refAdd(refcount, 1);
}

/**
* @berif �������ʼ��Ϊ0
*/
template <class _type>
void _Matrix<_type>::zeros()
{
	for (size_t i = 0; i < _size; ++i) {
		data[i] = 0;
	}
}

/**
* @berif ���·����ڴ沢��ʼ��Ϊ0
*/
template <class _type>
void _Matrix<_type>::zeros(int _rows, int _cols)
{
	create(_rows, _cols);

	for (size_t i = 0; i < _size; ++i) {
		data[i] = 0;
	}
}

/**
* @berif �������ʼ��Ϊ1
*/
template <class _type>
void _Matrix<_type>::ones()
{
	for (size_t i = 0; i < _size; ++i) {
		data[i] = 1;
	}
}

/**
* @berif ���·����ڴ沢��ʼ��Ϊ1
*/
template <class _type>
void _Matrix<_type>::ones(int _rows, int _cols)
{
	create(_rows, _cols);

	for (size_t i = 0; i < _size; ++i) {
		data[i] = 1;
	}
}


/**
* @berif �������ʼ��Ϊ��λ����
*/
template <class _type>
void _Matrix<_type>::eye()
{
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
	create(_rows, _cols);

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
* @berif ��ȸ��ƺ���
* @param[out] outputMatrix�����Ƶ�Ŀ�ľ��󣬻ᱻ���·����ڴ沢��������
*/
template <class _type>
void _Matrix<_type>::copyTo(_Matrix<_type> & outputMatrix) const
{
	outputMatrix.create(rows, cols);
	memcpy(outputMatrix.data, data, _size * sizeof(_type));
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


/**
* @berif ��������
*/
template <class _type>
_Matrix<_type>::~_Matrix()
{
	release();
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
		data = m.data;
		refcount = m.refcount;
		rows = m.rows;
		cols = m.cols;
	}

	return *this;
}


/**
* @berif ����mat = {1, 2, 3}�ĸ�ֵ��ʽ
*/
template <class _type>
_Matrix<_type>& _Matrix<_type>::operator = (std::initializer_list<_type> li)
{
	if (rows != 0 && cols != 0) {
		create(rows, cols);
	}
	else {
		create(1, li.size());
	}

	auto index = li.begin();
	auto end = li.end();
	for (size_t i = 0; i < _size; ++i, ++index) {
		if (index < end) {
			data[i] = *index;
		}
		else {
			data[i] = (_type)0;
		}
	}
	return *this;
}

template <class _type>
_Matrix<_type>& _Matrix<_type>::operator()(_type * InputArray, size_t _size)
{
	create(1, _size);
	for (size_t i = 0; i < _size; ++i)
		data[i] = InputArray[i];

	return *this;
}

template <class _type>
_Matrix<_type>& _Matrix<_type>::operator()(_type * InputArray, int _rows, int _cols)
{
	create(_rows, _cols);
	for (size_t i = 0; i < _size; ++i)
		data[i] = InputArray[i];

	return *this;
}







/**
* @berif ��������
* m x n������min(m, n)�������
*/
template <class _type>
_type _Matrix<_type>::rank()
{
	_type temp = (_type)0;
	return temp;
}

template <class _type>
_type _Matrix<_type>::tr()
{
	_type temp = (_type)0;
	int min_m_n = rows < cols ? rows : cols;
	for (int i = 0; i < min_m_n; ++i) {
		for (int j = 0; j < min_m_n; ++j) {
			if (i == j) {
				temp += data[i * cols + j];
			}
		}
	}
	return temp;
}

/**
* @berif ������������
*/
template <class _type>
std::ostream &operator<<(std::ostream & os, const _Matrix<_type> &item)
{
	os << '[';
	for (int i = 0; i < item.rows; ++i) {
		for (int j = 0; j < item.cols; ++j) {
			if(sizeof(_type) == 1)
				os << (int)item.data[i*item.cols + j];
			else
				os << item.data[i*item.cols + j];
			if (item.cols != j + 1)
				os << ',';
		}
		if (item.rows != i + 1)
			os << ';' << endl << ' ';
		else
			os << ']' << endl;
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
			if (m1.cols == m2.cols && m1.rows == m2.rows) {
				int i = 0;
				for (; i < m1.size(); ++i) {
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

template <class _type>
bool operator!=(const _Matrix<_type> &m1, const _Matrix<_type> &m2)
{
	return !(m1 == m2);
}

template <class _type>
_Matrix<_type> operator*(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	try {
		if (m1.cols != m2.rows) {
			throw;
		}
	}
	catch (exception) {
		_log_("����1���в����ھ���2������");
		return _Matrix();
	}

	_Matrix<_type> m(m1.rows, m2.cols);
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

template <class _type>
_Matrix<_type> operator+(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	try {
		if (m1.cols != m2.cols || m1.rows != m2.rows) {
			throw;
		}
	}
	catch (exception) {
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");
		return _Matrix();
	}

	_Matrix<_type> temp(m1.rows, m1.cols);

	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < temp.cols; ++j) {
			temp[i][j] = m1[i][j] + m2[i][j];
		}
	}
	return temp;
}

template <class _type>
_Matrix<_type> operator-(_Matrix<_type> &m1, _Matrix<_type> &m2)
{
	try {
		if (m1.cols != m2.cols || m1.rows != m2.rows) {
			throw;
		}
	}
	catch (exception) {
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");
		return _Matrix();
	}

	_Matrix<_type> temp(m1.rows, m1.cols);

	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < temp.cols; ++j) {
			temp[i][j] = m1[i][j] - m2[i][j];
		}
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

	return m;
}


/**
* @berif ת��
*/
template <class _type>
_Matrix<_type>  _Matrix<_type>::t()
{
	try {
		if (rows != cols) {
			throw;
		}
	}
	catch (exception) {
		_log_("rows != cols");
		return _Matrix();
	}

	_Matrix<_type> m(cols, rows);
	for (int i = 0; i < m.rows; ++i) {
		for (int j = 0; j < m.cols; ++j) {
			m[i][j] = (*this)[j][i];
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
	try {
		if (rows != m.rows || cols != m.cols) {
			throw;
		}
	}
	catch (exception) {
		_log_("���󲻷�������淶");
		return _Matrix();
	}

	_Matrix<_type> temp(m.rows, m.cols);

	for (size_t i = 0; i < _size; ++i) {
		temp.data[i] = data[i] * m.data[i];
	}

	return temp;
}

/**
* @berif ���
* @attention C = cross(A,B) returns the cross product of the vectors
A and B.  That is, C = A x B.  A and B must be 3 element
vectors.
*/
template <class _type>
_Matrix<_type> _Matrix<_type>::cross(_Matrix<_type> &m)
{
	try {
		if (rows != 1 || cols != 3 || m.rows != 1 || m.cols != 3) {
			throw;
		}
	}
	catch (exception) {
		_log_("���󲻷�������淶");
		return _Matrix();
	}

	_Matrix<_type> temp(1, 3);

	temp[0][0] = data[1] * m.data[2] - data[2] * m.data[1];
	temp[0][1] = data[2] * m.data[0] - data[0] * m.data[2];
	temp[0][2] = data[0] * m.data[1] - data[1] * m.data[0];

	return temp;
}

/**
* @berif �������ʱֻ���ھ����Ϊ3*3
*/
template <class _type>
_Matrix<_type> _Matrix<_type>::conv(_Matrix<_type> &m)
{
	try {
		// �������ҪΪ������������Ϊ����
		if (m.rows != m.cols || m.rows % 2 == 0) {
			throw;
		}
	}
	catch (exception) {
		_log_("���󲻷�������淶");
		return _Matrix();
	}

	_Matrix<_type> temp(rows, cols);
	temp.zeros();
	int depth = m.rows / 2;

	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < temp.cols; ++j) {
			// 
			for (int ii = 0; ii < m.rows; ++ii) {
				for (int jj = 0; jj < m.cols; ++jj) {
					temp[i][j] += (*this).at(i - m.rows / 2 + ii, j - m.cols / 2 + jj) * m[ii][jj];
				}
			}
		}
	}

	return temp;
}

/**
* @berif ����Խ����
*/
template <class _type>
inline _type _Matrix<_type>::at(int _rows, int _cols)
{
	if (_rows < 0 || _cols < 0 || _rows >= rows || _cols >= cols) {
		return 0.0;
	}
	else {
		return (*this)[_rows][_cols];
	}
}

template <class _type>
_Matrix<_type> operator*(_Matrix<_type> &m, _type delta)
{
	_Matrix<_type> temp(m.rows, m.cols);

	for (size_t i = 0; i < m.size(); ++i) {
		temp.data[i] = m.data[i] * delta;
	}

	return temp;
}

template <class _type>
_Matrix<_type> operator*(_type delta, _Matrix<_type> &m)
{
	return m*delta;
}

template <class _type>
_Matrix<_type> operator+(_Matrix<_type> &m, _type delta)
{
	_Matrix<_type> temp(m.rows, m.cols);

	for (size_t i = 0; i < m.size(); ++i) {
		temp.data[i] = m.data[i] + delta;
	}

	return temp;
}
template <class _type>
_Matrix<_type> operator+(_type delta, _Matrix<_type> &m)
{
	return m + delta;
}
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

#endif // ! __cplusplus

#endif