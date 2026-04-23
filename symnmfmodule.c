#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

/*
Expected Input:
list - Python list of lists
n, d - pointers for updating the dimensions of the matrix
Expected Output:
A flat C double array representing the same matrix
*/
static *double py_matrix_to_c(PyObject *list, int* n, int* d) {
    int i, j, rows, cols;

    int rows = (int)PyList_Size(list);
    int cols = (int)PyList_Size(PyList_GetItem(list, 0));

    *n = rows;
    *d = cols;

    double *matrix = malloc(rows * cols * sizeof(double));
    if (matrix == NULL) {
        printf("An Error Has Occurred\n");
        exit(1);   
    }

    for (i = 0; i < rows; i++) {
        PyObject *row = PyList_GetItem(list, i);
        for (j = 0; j < cols; j++) {
            PyObject *item = PyList_GetItem(row, j);
            matrix[i * cols + j] = PyFloat_AsDouble(item);
        }
    }
    return matrix;
}

/*
Expected Input:
matrix - C double array representing a matrix of size rows x cols
Expected Output:
A Python list of lists representing the same matrix
*/
static PyObject* array_to_py_matrix(double *matrix, int rows, int cols) {
    int i, j;
    PyObject *outer_list = PyList_New(rows);
    for (i = 0; i < rows; i++) {
        PyObject *inner_list = PyList_New(cols);
        for (j = 0; j < cols; j++) {
            PyObject *value = PyFloat_FromDouble(matrix[i * cols + j]);
            PyList_SetItem(inner_list, j, value);
        }
        PyList_SetItem(outer_list, i, inner_list);
    }
    return outer_list;
}

/*
Expected Input:
diagonal - C double array representing the diagonal values of a diagonal matrix of size n x n
Expected Output:
A Python list of lists representing the same matrix
*/
static PyObject* diag_array_to_py_matrix(double *diagonal, int n) {
    int i, j;
    PyObject *outer_list = PyList_New(n);
    for (i = 0; i < n; i++) {
        PyObject *inner_list = PyList_New(n);
        for (j = 0; j < n; j++) {
            PyObject *value;
            if(i == j) {
                value = PyFloat_FromDouble(diagonal[i]);
            } else {
                value = PyFloat_FromDouble(0.0);
            }
            PyList_SetItem(inner_list, j, value);
        }
        PyList_SetItem(outer_list, i, inner_list);
    }
    return outer_list;
}

static PyMethodDef symnmfMethods[] = {
    {"symnmf", (PyCFunction)symnmf, METH_VARARGS, PyDoc_STR("Calculate H matrix")},
    {"sym", (PyCFunction)sym, METH_VARARGS, PyDoc_STR("Calculate similarity matrix")},
    {"ddg", (PyCFunction)ddg, METH_VARARGS, PyDoc_STR("Calculate diagonal matrix")},
    {"norm", (PyCFunction)norm, METH_VARARGS, PyDoc_STR("Calculate W matrix")},
    {NULL, NULL, 0, NULL}
};

static struct PyModuleDef mysymnmf_module = {
    PyModuleDef_HEAD_INIT,
    "mysymnmf",
    NULL,
    -1,
    symnmfMethods
};

PyMODINIT_FUNC PyInit_mysymnmf(void) {
    PyObject *m;
    m = PyModule_Create(&mysymnmf_module);
    if (!m) {
        return NULL;
    }
    return m;
}