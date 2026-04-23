#ifndef SYMNMF_ALGO_H
#define SYMNMF_ALGO_H

#include <stdio.h>


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