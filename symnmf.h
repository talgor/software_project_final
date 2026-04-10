#ifndef SYMNMF_H
#define SYMNMF_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

double squared_euclidean_distance(double* p1, double* p2, int d);
void print_matrix(double* matrix, int rows, int cols);
void print_diag_matrix(double* diag, int n);
void mult_matrix(double* left, double* right, double* output, int left_rows, int right_cols, int shared_dim);
void transpose(double* matrix, double* transposed, int rows, int cols);
void mult_by_sqrt_of_diag(double* sym_matrix, double* diag, double* output, int n);

double* compute_symnmf_env(double* W, double* H,  int n, int k, int max_iter);
double* compute_symnmf(double* W, double* H,  int n, int k, int max_iter, double* new_H, double* numerator, double* denominator, double* transposed, double* denominator_part);
void update_H(double* old_H, double* new_H, double* W, int n, int k, double* numerator, double* denominator, double* transposed, double* denominator_part);
double* compute_sym(double* datapoints, int n, int d);
double* compute_ddg(double* sym_matrix, int n);
double* compute_norm(double* sym_matrix, int n);
void extract_n_d(FILE* ifp, int* out_n, int* out_d);
void read_datapoints_from_file(FILE* ifp, int n, int d, double* datapoints);
double* read_file_env(char* filename, int* out_n, int* out_d);
#endif