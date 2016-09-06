#include "zmatrix.h"

using namespace std;

/**
 * @berif �������ʼ��Ϊ�վ���
 */
void Matrix::initEmpty()
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
void Matrix::create(int _rows, int _cols)
{
	_log_("Matrix create.");

	rows = _rows;
	cols = _cols;
	_size = rows * cols;

	// 
	release();

	// ����
	data = new double[_rows * _cols];
	refcount = new int(1);
}

/**
 * @berif �������ü�����ֵ
 */
int Matrix::refAdd(int *addr, int delta)
{
	int temp = *addr;
	*addr += delta;
	return temp;
}

/**
 * @berif �ͷ���Դ
 * @attention �������Դ�ɸú������Ʋ��ͷ�
 */
void Matrix::release()
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
Matrix::Matrix()
{
	_log_("Matrix construct without params.");
	initEmpty();
}

/**
 * @berif ���캯��
 * @param[in] _rows������
 * @param[in] _cols������
 */
Matrix::Matrix(int _rows, int _cols)
{
	_log_("Matrix construct with params.");
	initEmpty();
	create(_rows, _cols);
}
/**
 * @berif ��������
 * @attention ����һ��ǳ����
 */
Matrix::Matrix(const Matrix& m)
	:rows(m.rows),cols(m.cols),data(m.data),refcount(m.refcount)
{
	_log_("Matrix copying function.");
	if (refcount)
		refAdd(refcount, 1);
}

/**
 * @berif �������ʼ��Ϊ0
 */
void Matrix::zeros()
{
	for (int i = 0; i < _size; ++i) {
		data[i] = 0;
	}
}

/**
 * @berif ���·����ڴ沢��ʼ��Ϊ0
 */
void Matrix::zeros(int _rows, int _cols)
{
	create(_rows, _cols);

	for (int i = 0; i < _size; ++i) {
		data[i] = 0;
	}
}

/**
 * @berif �������ʼ��Ϊ1
 */
void Matrix::ones()
{
	for (int i = 0; i < _size; ++i) {
		data[i] = 1;
	}
}

/**
 * @berif ���·����ڴ沢��ʼ��Ϊ1
 */
void Matrix::ones(int _rows, int _cols)
{
	create(_rows, _cols);

	for (int i = 0; i < _size; ++i) {
		data[i] = 1;
	}
}


/**
 * @berif �������ʼ��Ϊ��λ����
 */
void Matrix::eye()
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
void Matrix::eye(int _rows, int _cols)
{
	create(_rows, _cols);

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			if(i  == j)
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
void Matrix::copyTo(Matrix & outputMatrix) const
{
	outputMatrix.create(rows, cols);
	memcpy(outputMatrix.data, data, _size*sizeof(double));
}

/**
 * @berif ��ȸ��ƺ���
 * @ret ������ʱ����Ŀ���
 */
Matrix Matrix::clone() const
{
	Matrix m;
	copyTo(m);
	return m;
}


/**
 * @berif ��������
 */
Matrix::~Matrix()
{
	release();
}

/**
* @berif ��ֵ����
* @attention ����һ��ǳ����
*/
Matrix& Matrix::operator=(const Matrix &m)
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
Matrix& Matrix::operator = (std::initializer_list<double> li)
{
	if (rows != 0 && cols != 0) {
		create(rows, cols);
	}
	else {
		create(1, li.size());
	}
	
	auto index = li.begin();
	auto end = li.end();
	for (int i = 0; i < _size; ++i, ++index) {
		if (index < end) {
			data[i] = *index;
		}
		else {
			data[i] = 0.0f;
		}
	}
	return *this;
}

Matrix& Matrix::operator()(double * InputArray, size_t _size)
{
	create(1, _size);
	for (int i = 0; i < _size; ++i)
		data[i] = InputArray[i];

	return *this;
}

Matrix& Matrix::operator()(double * InputArray, int _rows, int _cols)
{
	create(_rows, _cols);
	for (int i = 0; i < _size; ++i)
		data[i] = InputArray[i];

	return *this;
}







/**
 * @berif ��������
 * m x n������min(m, n)�������
 */
double	Matrix::rank()
{
	double temp = 0.0f;
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
ostream &operator<<(ostream & os, const Matrix &item)
{
	os << '[';
	for (int i = 0; i < item.rows; ++i) {
		for (int j = 0; j < item.cols; ++j) {
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
bool operator==(const Matrix &m1, const Matrix &m2)
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

bool operator!=(const Matrix &m1, const Matrix &m2)
{
	return !(m1 == m2);
}


Matrix operator*(Matrix &m1, Matrix &m2)
{
	try {
		if (m1.cols != m2.rows) {
			throw;
		}
	}
	catch (exception){
		_log_("����1���в����ھ���2������");
		return Matrix();
	}

	Matrix m(m1.rows, m2.cols);
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

Matrix operator+(Matrix &m1, Matrix &m2)
{
	try {
		if (m1.cols != m2.cols || m1.rows != m2.rows) {
			throw;
		}
	}
	catch (exception) {
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");
		return Matrix();
	}

	Matrix temp(m1.rows, m1.cols);

	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < temp.cols; ++j) {
			temp[i][j] = m1[i][j] + m2[i][j];
		}
	}
	return temp;
}

Matrix operator-(Matrix &m1, Matrix &m2)
{
	try {
		if (m1.cols != m2.cols || m1.rows != m2.rows) {
			throw;
		}
	}
	catch (exception) {
		_log_("m1.cols != m2.cols || m1.rows != m2.rows");
		return Matrix();
	}

	Matrix temp(m1.rows, m1.cols);

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
Matrix Matrix::inv()
{
	Matrix m(cols,rows);
	
	return m;
}


/**
 * @berif ת��
 */
Matrix  Matrix::t()
{
	try {
		if (rows != cols) {
			throw;
		}
	}
	catch (exception) {
		_log_("rows != cols");
		return Matrix();
	}

	Matrix m(cols, rows);
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
Matrix Matrix::dot(Matrix &m)
{
	Matrix temp(3, 3);
	return temp;
}

/**
 * @berif ���
 * @attention C = cross(A,B) returns the cross product of the vectors
    A and B.  That is, C = A x B.  A and B must be 3 element
    vectors.
 */
Matrix Matrix::cross(Matrix &m)
{
	try {
		if (rows != 1 || cols != 3 || m.rows != 1 || m.cols != 3) {
			throw;
		}
	}
	catch (exception) {
		_log_("���󲻷�������淶");
		return Matrix();
	}

	Matrix temp(1, 3);

	temp[0][0] = data[1] * m.data[2] - data[2] * m.data[1];
	temp[0][1] = data[2] * m.data[0] - data[0] * m.data[2];
	temp[0][2] = data[0] * m.data[1] - data[1] * m.data[0];

	return temp;
}

/**
 * @berif �������ʱֻ���ھ����Ϊ3*3
 */
Matrix Matrix::conv(Matrix &m)
{
	try {
		if (m.rows != m.cols || m.rows % 2 == 0) {
			throw;
		}
	}
	catch (exception) {
		_log_("���󲻷�������淶");
		return Matrix();
	}

	Matrix temp(rows, cols);
	int depth = m.rows / 2;

	for (int i = 0; i < temp.rows; ++i) {
		for (int j = 0; j < temp.cols; ++j) {
			temp[i][j] = (*this).at(i - 1, j - 1) * m[0][0] + (*this).at(i - 1, j) * m[0][1] + (*this).at(i - 1, j + 1) * m[0][2]
				+ (*this).at(i, j - 1) * m[1][0] + (*this).at(i, j) * m[1][1] + (*this).at(i, j + 1) * m[1][2]
				+ (*this).at(i + 1, j - 1) * m[2][0] + (*this).at(i + 1, j) * m[2][1] + (*this).at(i + 1, j + 1) * m[2][2];
		}
	}

	return temp;
}

/**
 * @berif ����Խ����
 */
double Matrix::at(int _rows, int _cols)
{
	if (_rows < 0 || _cols < 0 || _rows >= rows || _cols >= cols) {
		return 0.0;
	}
	else {
		return (*this)[_rows][_cols];
	}
}