#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf.h"

static PyObject* norm(PyObject *self, PyObject *args);
static PyObject* ddg(PyObject *self, PyObject *args);
static PyObject* sym(PyObject *self, PyObject *args);
static PyObject* symnmf(PyObject *self, PyObject *args);
static double* py_matrix_to_c(PyObject* matrix, int* n, int* d);
static PyObject* array_to_py_matrix(double* c_array, int n, int d);
static PyObject* diag_array_to_py_matrix(double* c_array, int n);

static PyObject* norm(PyObject *self, PyObject *args){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_norm_matrix;
    double* c_datapoints;
    double* sym_matrix;
    double* norm_matrix;
    int n = 0, d = 0;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints, &n, &d);
    sym_matrix = compute_sym(c_datapoints, n, d);
    free(c_datapoints);
    norm_matrix = compute_norm(sym_matrix, n);
    free(sym_matrix);
    py_norm_matrix = array_to_py_matrix(norm_matrix, n, n);
    free(norm_matrix);
    return py_norm_matrix;
}

static PyObject* ddg(PyObject *self, PyObject *args){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_ddg_matrix;
    double* c_datapoints;
    double* sym_matrix;
    double* ddg_matrix;
    int n = 0, d = 0;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints, &n, &d);
    sym_matrix = compute_sym(c_datapoints, n, d);
    free(c_datapoints);
    ddg_matrix = compute_ddg(sym_matrix, n);
    free(sym_matrix);
    py_ddg_matrix = diag_array_to_py_matrix(ddg_matrix, n);
    free(ddg_matrix);
    return py_ddg_matrix;
}

static PyObject* sym(PyObject *self, PyObject *args){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_sym_matrix;
    double* c_datapoints;
    double* sym_matrix;
    int n = 0, d = 0;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints, &n, &d);
    sym_matrix = compute_sym(c_datapoints, n, d);
    free(c_datapoints);
    py_sym_matrix = array_to_py_matrix(sym_matrix, n, n);
    free(sym_matrix);
    return py_sym_matrix;
}


static PyObject* symnmf(PyObject *self, PyObject *args){
    /*gets W and initial_H*/
    PyObject *py_W;
    PyObject *initial_H;
    PyObject *final_H;
    double* c_H;
    double* c_final_H;
    double* c_W;
    int dummy1;
    int dummy2;
    int n;
    int k;
    int max_iter = 300;

    if(!PyArg_ParseTuple(args, "OOii", &py_W, &initial_H, &n, &k)) {
        return NULL;
    }

    c_H = py_matrix_to_c(initial_H, &dummy1, &dummy2);
    c_W = py_matrix_to_c(py_W, &dummy1, &dummy2);
    c_final_H = compute_symnmf_env(c_W, c_H,  n, k, max_iter);
    final_H = array_to_py_matrix(c_final_H, n, k);
    free(c_H);
    return final_H;
}

/*
Expected Input:
list - Python list of lists
n, d - pointers for updating the dimensions of the matrix
Expected Output:
A flat C double array representing the same matrix
*/
static double* py_matrix_to_c(PyObject *list, int* n, int* d) {
    int i, j, rows, cols;

    rows = (int)PyList_Size(list);
    cols = (int)PyList_Size(PyList_GetItem(list, 0));

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
