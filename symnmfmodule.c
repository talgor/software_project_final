#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf_algo.h"




static PyObject* norm(PyObject *self, PyObject *args);
static PyObject* ddg(PyObject *self, PyObject *args);
static PyObject* sym(PyObject *self, PyObject *args);
static PyObject* symnmf(PyObject *self, PyObject *args);
double* py_matrix_to_c(PyObject* matrix, int* n, int* d);
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
