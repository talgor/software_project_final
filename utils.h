#ifndef UTILS_H
#define UTILS_H

void print_matrix(double* matrix, int rows, int cols);
void print_diag_matrix(double* diag, int n);
void mult_by_sqrt_of_diag(double* matrix, double* diag, double* output, int n);
void mult_matrix(double* left, double* right, double* output, int left_rows, int right_cols, int shared_dim);
void transpose(double* matrix, double* output, int rows, int cols);
double squared_euclidean_distance(double* p1, double* p2, int d);

#endif