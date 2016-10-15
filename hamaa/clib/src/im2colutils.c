#include "D:/Anaconda2/include/Python.h"
#include "D:/Anaconda2/Lib/site-packages/numpy/core/include/numpy/arrayobject.h"

PyArrayObject *im2col_HW(PyArrayObject *x, int KH, int KW, int stride)
{
    assert(PyArray_TYPE(x) == NPY_DOUBLE);
    // ��ȡ��ά����x����״
    int H = PyArray_DIM(x, 0);
    int W = PyArray_DIM(x, 1);

    // ����x��w���о����Ľ������״
    int CH = (H - KH) / stride + 1;
    int CW = (W - KW) / stride + 1;

    // ��x����2άim2colչ�����col_x�Ĵ�С
    int IH = KH * KW;
    int IW = CH * CW;

    // ����numpy.ndarray����col_x����im2colչ�����x
    int nd = 2;
    npy_intp dims[] = {IH, IW};
    PyArrayObject *col_x = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);

    // ������ʾ�������еĵ�crow�е�ccol��
    int crow, ccol;

    // ������ʾx�еĵ�row�е�col��
    int row, col;

    // ������ʾ��w��x�����������ʱ��w�����Ͻ���x�е�λ��
    int row_start, col_start;

    // ������ʾim2colչ��x��col_x�еĵ�irow�е�icol��
    int irow, icol;

    // ��w��x�ϻ�����ĳһ��λ��ʱ����i��j������x��w�ص����������
    int i, j;

    // ��ǰ��������ת������ΪPyArray_DATA()��PyArray_GETPTR1/2/3/4()
    // ����������ȡ������ָ�붼��void *����
    npy_double *x_data = PyArray_DATA(x);
    npy_double *col_x_data = PyArray_DATA(col_x);

    // ��ʼim2col����
    icol = 0;
    for(crow = 0; crow < CH; ++crow)
    {
        for(ccol = 0; ccol < CW; ++ccol)
        {
            row_start = crow * stride;
            col_start = ccol * stride;
            irow = 0;
            for(i = row_start; i < row_start + KH; ++i)
            {
                for(j = col_start; j < col_start + KW; ++j)
                {
                    col_x_data[irow * IW + icol] = x_data[i * W + j];
                    // �������ǰ���������ݽ�����ʽ��������ת��
//                    *((npy_double *)(PyArray_GETPTR2(col_x, irow, icol))) = \
//                    *((npy_double *)(PyArray_GETPTR2(x, i, j)));
                    ++irow;
                }
            }
            ++icol;
        }
    }
    return col_x;
}

PyArrayObject *im2col_NCHW(PyArrayObject *x, int KH, int KW, int stride)
{
    assert(PyArray_TYPE(x) == NPY_DOUBLE);
    // ��ȡ��ά����x����״
    int N = PyArray_DIM(x, 0);
    int C = PyArray_DIM(x, 1);
    int H = PyArray_DIM(x, 2);
    int W = PyArray_DIM(x, 3);

    // ����x��w���о����Ľ������״
    int CH = (H - KH) / stride + 1;
    int CW = (W - KW) / stride + 1;

    // ��x����2άim2colչ�����col_x�Ĵ�С
    int IH = C * KH * KW;
    int IW = N * CH * CW;

    // ����numpy.ndarray����col_x����im2colչ�����x
    int nd = 2;
    npy_intp dims[] = {IH, IW};
    PyArrayObject *col_x = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);


    // ������ʾ�������еĵ�crow�е�ccol��
    int crow, ccol;

    // ������ʾx�еĵ�row�е�col��
    int row, col;

    // ������ʾ��w��x�����������ʱ��w�����Ͻ���x�е�λ��
    int row_start, col_start;

    // ������ʾim2colչ��x��col_x�еĵ�irow�е�icol��
    int irow, icol;

    // ������ʾ��x[n][c]����im2colչ��ʱ��������col_x�е����Ͻ�����
    int irow_start, icol_start;

    // ��w��x�ϻ�����ĳһ��λ��ʱ����i��j������x��w�ص����������
    int i, j;

    // ������ʾx�еĵ�n��ͼƬ�ĵ�c��ͨ��
    int n, c, n_start, c_start;

    // ��ǰ��������ת������ΪPyArray_DATA()��PyArray_GETPTR1/2/3/4()
    // ����������ȡ������ָ�붼��void *����
    npy_double *x_data = PyArray_DATA(x);
    npy_double *col_x_data = PyArray_DATA(col_x);

    // ��x�еĵ�n��ͼƬ�ĵ�c��ͨ������im2colչ��
    for(n = 0; n < N; ++n)
    {
        for(c = 0; c < C; ++c)
        {
            // ��x[n][c]չ�����im2col������col_x�е�λ�õ����Ͻ�����
            irow_start = c * KH * KW;
            icol_start = n * CH * CW;

            // ��ʼ��x[n][c]����im2col����
            // ��im2col_HW�����У��˴�Ϊicol = 0;
            icol = icol_start;
            for(crow = 0; crow < CH; ++crow)
            {
                for(ccol = 0; ccol < CW; ++ccol)
                {
                    // ��im2col_HW�����У��˴�Ϊirow = 0;
                    irow = irow_start;
                    row_start = crow * stride;
                    col_start = ccol * stride;
                    for(i = row_start; i < row_start + KH; ++i)
                    {
                        for(j = col_start; j < col_start + KW; ++j)
                        {
                            col_x_data[irow * IW + icol] = x_data[n*C*H*W + c*H*W + i*W+j];

                            // �������ǰ���������ݽ�����ʽ��������ת��
                            // ����Ҫ��ÿ��ȡ����ʱ��������ת��
//                            *((npy_double *)(PyArray_GETPTR2(col_x, irow, icol))) = \
//                            *((npy_double *)(PyArray_GETPTR4(x, n, c, i, j)));
                            ++irow;
                        }
                    }
                    ++icol;
                }
            }
        }
    }

    return col_x;
}



PyArrayObject *im2col_NCHW_memcpy(PyArrayObject *x, int KH, int KW, int stride)
{
    assert(PyArray_TYPE(x) == NPY_DOUBLE);
    // ��ȡ��ά����x����״
    int N = PyArray_DIM(x, 0);
    int C = PyArray_DIM(x, 1);
    int H = PyArray_DIM(x, 2);
    int W = PyArray_DIM(x, 3);

    // ����x��w���о����Ľ������״
    int CH = (H - KH) / stride + 1;
    int CW = (W - KW) / stride + 1;

    // ��x����2άim2colչ�����row_x�Ĵ�С
    int IH = N * CH * CW;
    int IW = C * KH * KW;

    // ����numpy.ndarray����row_x����ʹ��im2colչ��xʱcol_x��ת��
    int nd = 2;
    npy_intp dims[] = {IH, IW};
    PyArrayObject *row_x = PyArray_SimpleNew(nd, dims, NPY_DOUBLE);

    // ������im2col���
    PyArrayObject *col_x;

    // ������ʾ�������еĵ�crow�е�ccol��
    int crow, ccol;

    // ������ʾx�еĵ�row�е�col��
    int row, col;

    // ������ʾ��w��x�����������ʱ��w�����Ͻ���x�е�λ��
    int row_start, col_start;

    // ������ʾim2colչ��x��row_x�еĵ�irow�е�icol��
    int irow, icol;

    // ������ʾ��x[n][c]����im2colչ��ʱ��������row_x�е����Ͻ�����
    int irow_start, icol_start;

    // ��w��x�ϻ�����ĳһ��λ��ʱ����i��j������x��w�ص����������
    int i, j;

    // ������ʾx�еĵ�n��ͼƬ�ĵ�c��ͨ��
    int n, c, n_start, c_start;

    // ��ǰ��������ת������ΪPyArray_DATA()��PyArray_GETPTR1/2/3/4()
    // ����������ȡ������ָ�붼��void *����
    npy_double *x_data = PyArray_DATA(x);
    npy_double *row_x_data = PyArray_DATA(row_x);

    // ��x�еĵ�n��ͼƬ�ĵ�c��ͨ������im2colչ��
    for(n = 0; n < N; ++n)
    {
        for(c = 0; c < C; ++c)
        {
            // ��x[n][c]չ�����im2col������row_x�е�λ�õ����Ͻ�����
            irow_start = n * CH * CW;
            icol_start = c * KH * KW;

            // ��ʼ��x[n][c]����im2col����
            irow = irow_start;
            for(crow = 0; crow < CH; ++crow)
            {
                for(ccol = 0; ccol < CW; ++ccol)
                {
                    // ��im2col_HW�����У��˴�Ϊirow = 0;
                    icol = icol_start;
                    row_start = crow * stride;
                    col_start = ccol * stride;
                    for(i = row_start; i < row_start + KH; ++i)
                    {
                        // ʹ��memcpy�����ַ�����Ӧ�Ծ���˱Ƚϴ��ʱ��Ч��Ҫ��������Ҫ����
                        memcpy(row_x_data+irow*IW+icol, x_data+n*C*H*W+c*H*W+i*W+col_start, KW*sizeof(npy_double));
                        icol += KW;
                    }
                    ++irow;
                }
            }
        }
    }
    col_x = PyArray_Transpose(row_x, NULL);
    Py_DECREF(row_x);
    return col_x;
}

PyObject *wrap_im2col_HW(PyObject *self, PyObject *args)
{
    PyArrayObject *x, *col_x;
    int KH, KW, stride;
    if(!PyArg_ParseTuple(args, "Oiii", &x, &KH, &KW, &stride))
        return NULL;
    col_x = im2col_HW(x, KH, KW, stride);
    return col_x;
}

PyObject *wrap_im2col_NCHW(PyObject *self, PyObject *args)
{
    PyArrayObject *x, *col_x;
    int KH, KW, stride;
    if(!PyArg_ParseTuple(args, "Oiii", &x, &KH, &KW, &stride))
        return NULL;
    col_x = im2col_NCHW(x, KH, KW, stride);
    return col_x;
}

PyObject *wrap_im2col_NCHW_memcpy(PyObject *self, PyObject *args)
{
    PyArrayObject *x, *col_x;
    int KH, KW, stride;
    if(!PyArg_ParseTuple(args, "Oiii", &x, &KH, &KW, &stride))
        return NULL;
    col_x = im2col_NCHW_memcpy(x, KH, KW, stride);
    return col_x;
}

static PyMethodDef methods[] = {
    {"im2col_HW", wrap_im2col_HW, METH_VARARGS, "no doc"},
    {"im2col_NCHW", wrap_im2col_NCHW, METH_VARARGS, "no doc"},
    {"im2col_NCHW_memcpy", wrap_im2col_NCHW_memcpy, METH_VARARGS, "no doc"},
    {NULL, NULL, 0, NULL},
};


PyMODINIT_FUNC initim2colutils()
{
    Py_InitModule("im2colutils", methods);
    import_array();
}

