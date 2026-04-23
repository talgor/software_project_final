#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "symnmf_algo.h"

static int N = 0;
static int D = 0;



static PyObject* norm(PyObject*);
static PyObject* ddg(PyObject*);
static PyObject* sym(PyObject*);
static PyObject* symnmf(PyObject*, PyObject*);
double* py_matrix_to_c(PyObject*); /*should update N and D*/
static PyObject* array_to_py_matrix(double*, int, int);
static PyObject* diag_array_to_py_matrix(double*, int);




static PyObject* norm(PyObject*){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_norm_matrix;
    double* c_datapoints;
    double* sym_matrix;
    double* norm_matrix;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints);
    sym_matrix = compute_sym(datapoints, N, D);
    free(datapoints);
    norm_matrix = compute_norm(sym_matrix, N);
    free(sym_matrix);
    py_norm_matrix = array_to_py_matrix(norm_matrix, N, N);
    free(norm_matrix)
    return py_norm_matrix;
}

static PyObject* ddg(PyObject*){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_ddg_matrix;
    double* c_datapoints;
    double* sym_matrix;
    double* ddg_matrix;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints);
    sym_matrix = compute_sym(datapoints, N, D);
    free(datapoints);
    ddg_matrix = compute_ddg(sym_matrix, N);
    free(sym_matrix);
    py_ddg_matrix = diag_array_to_py_matrix(ddg_matrix, N);
    free(ddg_matrix)
    return py_ddg_matrix;
}

static PyObject* sym(PyObject*){
    /*gets list of datapoints*/
    PyObject *py_datapoints;
    PyObject *py_sym_matrix;
    double* c_datapoints;
    double* sym_matrix;

    if(!PyArg_ParseTuple(args, "O", &py_datapoints)) {
        return NULL;
    }

    c_datapoints = py_matrix_to_c(py_datapoints);
    sym_matrix = compute_sym(datapoints, N, D);
    free(datapoints);
    py_sym_matrix = array_to_py_matrix(sym_matrix, N, N);
    free(sym_matrix)
    return py_sym_matrix;
}


static PyObject* symnmf(PyObject*, PyObject*){
    /*gets W and initial_H*/
    PyObject *py_W;
    PyObject *initial_H;
    PyObject *final_H;
    double* c_initial_H;
    double* c_W;
    double* c_final_H;
    int n;
    int k;
    int max_iter = 300;

    if(!PyArg_ParseTuple(args, "OOii", &py_W, &initial_H, &n, &k)) {
        return NULL;
    }

    c_initial_H = py_matrix_to_c(initial_H);
    c_W = py_matrix_to_c(py_W);
    c_final_H = compute_symnmf_env(c_W, c_initial_H,  n, k, max_iter);
    free(c_initial_H);
    free(c_W);
    final_H = array_to_py_matrix(c_final_H);
    free(c_final_H);
    return final_H;
}
