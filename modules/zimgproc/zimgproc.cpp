/**
 ******************************************************************************
 * @file    zimgproc.cpp
 * @author  zlq
 * @version V1.0
 * @date    2016.9.14
 * @brief   �������޹ص�ͼ��������ʵ��
 ******************************************************************************
 * @attention
 *
 *
 ******************************************************************************
 */
#include "zimgproc.h"
#include "zcore\debug.h"
#include "zmatch\zmatch.h"

namespace z{

/**
 * @berif openCV�е�Mat��ת��ΪMatrix8u��
 */
Matrix8u Mat2Matrix8u(cv::Mat & mat)
{
	Matrix8u temp(mat.rows, mat.cols, mat.channels());
	memcpy(temp.data, mat.data, temp.size()*temp.chs);

	return temp;
}

/**
* @berif ���µߵ�ͼ��
*/
void convertImage(Matrix8u *src, Matrix8u *dst, int flags)
{
	if (!dst->equalSize(*src))
		dst->create(src->rows, src->cols, src->chs);

	int rows = src->rows;
	int cols = src->cols;

	for (int i = 0; i < rows; ++i) {
		for (int j = 0; j < cols; ++j) {
			for (int k = 0; k < src->chs; ++k) {
				dst->ptr(i, j)[k] = src->ptr(rows - i - 1, j)[k];
			}
		}
	}
}

void copyToArray(Matrix8u &src, char * arr)
{
	int dataSize = src.size()* src.chs;
	for (int i = 0; i < dataSize; ++i) {
		arr[i] = src.data[i];
	}
}
/**
 * @berif ��ȡ���ڽ��и�˹�˲��ĸ�˹��
 */
Matrix Gassion(z::Size ksize, double sigmaX, double sigmaY)
{
	if (ksize.width != ksize.height || ksize.width % 2 != 1) {
		_log_("ksize.width != ksize.height || ksize.width % 2 != 1");
	}

	if (sigmaX == 0) sigmaX = ksize.width / 2.0;
	if (sigmaY == 0) sigmaY = ksize.height / 2.0;

	int x = ksize.width / 2;
	int y = ksize.height / 2;
	double z;

	Matrix kernel(ksize);

	for (int i = 0; i < kernel.rows; ++i) {
		for (int j = 0; j < kernel.cols; ++j) {
			z = (i - x) * (i - x)/sigmaX + (j - y) * (j - y)/sigmaY;
			kernel[i][j] = exp(-z);
		}
	}

	double a = 1.0 / kernel[0][0];

	for (int i = 0; i < kernel.rows; ++i) {
		for (int j = 0; j < kernel.cols; ++j) {
			kernel[i][j] = int(kernel[i][j] * a);
		}
	}
	return kernel;
}



/**
 * @berif 1D or 2D ��ɢ����Ҷ�任
 * @param src
 * @param dst
 */
void _dft(Matrix & src, Matrix & dst, Ft ft)
{
	Matrix temp(src.rows, src.cols, 2);
	Matrix end(src.rows, src.cols, 2);

	// �������
	const int N = src.cols;
	for (int i = 0; i < src.rows; ++i) {
		for (int v = 0; v < N; ++v) {
			Complex mt(0, 0);
			for (int n = 0; n < N; ++n) {
				double beta = (2 * Pi * v * n) / N;
                Complex w(cos(beta), ft * sin(beta));
                Complex g(src.ptr(i, n)[0], src.ptr(i, n)[1]);
                mt += w * g;
			}
            if(ft == DFT){
                temp.ptr(i, v)[0] = mt.re;
                temp.ptr(i, v)[1] = mt.im;
            }
            else {
                temp.ptr(i, v)[0] = mt.re / N;
                temp.ptr(i, v)[1] = mt.im / N;
            }

		}
	}

    if (src.rows < 2) {
        dst = temp;
        return;
    }

	// ���м���
	const int M = src.rows;
	for (int j = 0; j < src.cols; ++j) {
		for (int u = 0; u < M; ++u) {
			Complex mt(0,0);
			for (int m = 0; m < M; ++m) {
				double alpha = (2 * Pi * u * m) / M;

                Complex w(cos(alpha), ft * sin(alpha));
                Complex g(temp.ptr(m, j)[0], temp.ptr(m, j)[1]);
                mt += w * g;
			}
            if(ft == DFT){
                end.ptr(u, j)[0] = mt.re;
                end.ptr(u, j)[1] = mt.im;
            }
            else{
                end.ptr(u, j)[0] = mt.re / M;
                end.ptr(u, j)[1] = mt.im / M;
            }

		}
	}
    dst = end;
}


/**
 * @berif 1D��2D��ɢ����Ҷ�任
 */
void dft(Matrix & src, Matrix & dst)
{
	Matrix gx, gRe = src;
	Matrix gIm(src.rows, src.cols, 1);
	gIm.zeros();
	merge(gRe, gIm, gx);

	_dft(gx, dst, DFT);
}

void idft(Matrix & src, Matrix & dst)
{
    std::vector<Matrix> mv;
    Matrix temp;
    _dft(src, temp, IDFT);
    spilt(temp, mv);

    dst = mv.at(0);
}


/**
 * @berif ��ȡ��2FFT����ľ���ߴ�
 * @param cols
 * @return
 */
int getIdealCols(int cols)
{
    if(cols < 1){
        _log_("Error!");
    }

	int temp = 1;
	while (cols > temp) {
		temp *= 2;
	}
	return temp;
}
int getIdealRows(int rows)
{
	return getIdealCols(rows);
}

/**
 * @berif ��_Matrix������еĶ����Ʒ�ת
 * @param src
 */
void bitRevCols(Matrix & src)
{
	int32_t HELF_N = src.cols >> 1;
    int32_t k;

	for(int32_t i = 1, j = HELF_N; i < src.cols  - 1; ++i){
		if(i < j){
			for(int32_t row = 0; row < src.rows; ++row){
				src.swap(row, i, row, j);
			}
		}

        k = HELF_N;
        while(j >= k){
            j -= k;
            k >>= 1;
        }
        if(j < k) j += k;
	}
}

/**
 * @berif ��_Matrix�����������ж����Ʒ�ת
 * @param src
 */
void bitRevRows(Matrix & src)
{
    int32_t HELF_N = src.rows >> 1;
    int32_t k;

    for(int32_t i = 1, j = HELF_N; i < src.rows - 1; ++i){
        if(i < j){
            for(int32_t col = 0; col < src.cols; ++col){
                src.swap(i, col, j, col);
            }
        }

        k = HELF_N;
        while(j >= k){
            j -= k;
            k >>= 1;
        }
        if(j < k) j += k;
    }
}


/**
 * @berif 1D or 2D ��2FFT, �͵ؼ���
 * @param src
 */
void _fft(Matrix & src, Ft ft)
{
    // �����Ʒ�ת���з�ת
    bitRevCols(src);

	for (int i = 0; i < src.rows; ++i) {

        // �����㷨
		for (int l = 2; l <= src.cols; l <<= 1) {    // ��Ҫlog2(N)��
            for(int k = 0; k < src.cols; k += l) {
                for(int n = 0; n < l >> 1; ++n) {

                    // Wn��ת����
                    Complex W(cos((2 * Pi * n) / l), ft * sin((2 * Pi * n) / l));

                    // ���µ���
                    Complex up(src.ptr(i, k + n)[0], src.ptr(i, k + n)[1]);
                    Complex down(src.ptr(i, k + n + l/2)[0], src.ptr(i, k + n + l/2)[1]);

                    Complex m = down * W;
                    down = up - m;
                    up = up + m;

                    src.ptr(i, k + n)[0] = up.re;
                    src.ptr(i, k + n)[1] = up.im;

                    src.ptr(i, k + n + l/2)[0] = down.re;
                    src.ptr(i, k + n + l/2)[1] = down.im;

                } // !for(n)
            } // !for(k)
		} // !for(l)
	} // !for(i)

    // �����1D�ľ����򷵻�
    if(src.rows < 2) return;

    // �з�ת
    bitRevRows(src);

    for(int j = 0; j < src.cols; ++j){

        for (int l = 2; l <= src.rows; l <<= 1) {    // ��Ҫlog2(N)��
            for (int k = 0; k < src.rows; k += l) {
                for (int n = 0; n < l >> 1; ++n) {

                    // W = cos(2 * Pi / N) - sin(2 * Pi / N)
                    Complex W(cos((2 * Pi * n) / l), ft * sin((2 * Pi * n) / l));

                    Complex up(src.ptr(k + n, j)[0], src.ptr(k + n, j)[1]);
                    Complex down(src.ptr(k + n + l/2, j)[0], src.ptr(k + n + l/2, j)[1]);

                    Complex m = down * W;
                    down = up - m;
                    up = up + m;

                    src.ptr(k + n, j)[0] = up.re;
                    src.ptr(k + n, j)[1] = up.im;

                    src.ptr(k + n + l/2, j)[0] = down.re;
                    src.ptr(k + n + l/2, j)[1] = down.im;
                }
            }
        }
    }
}



void fft(Matrix & src, Matrix & dst)
{
	Matrix gRe;
	int fft_rows = getIdealRows(src.rows);
	int fft_cols = getIdealCols(src.cols);

    // ����ԭͼ��
	copyMakeBorder(src, gRe, 0, fft_rows - src.rows, 0, fft_cols - src.cols);

    // ��������
	Matrix gIm(fft_rows, fft_cols, 1);
    gIm.zeros();

    // ������ʵ�����ֺϳɣ�FFT�����ͼ��
	Matrix gx;
	merge(gRe, gIm, gx);

    // ���п��ٸ���Ҷ�任
	_fft(gx, DFT);
    dst = gx;
}

void ifft(Matrix & src, Matrix & dst)
{
    std::vector<Matrix> mv;
    _fft(src, IDFT);
    spilt(src, mv);
    dst = mv.at(0);
}


}