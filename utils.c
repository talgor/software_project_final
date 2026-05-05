#include "utils.h"


double squared_euclidean_distance(double* p1, double* p2, int d);
void print_matrix(double* matrix, int rows, int cols);
void print_diag_matrix(double* diag, int n);
void mult_matrix(double* left, double* right, double* output, int left_rows, int right_cols, int shared_dim);
void transpose(double* matrix, double* transposed, int rows, int cols);
void mult_by_sqrt_of_diag(double* sym_matrix, double* diag, double* output, int n);

/*
Expected Input:
p1, p2 - pointers to two arrays of doubles representing vectors in d-dimensional space
d - int representing the dimension of the vectors
Expected Output:
A double representing the squared Euclidean distance between the two points
*/
double squared_euclidean_distance(double* p1, double* p2, int d) {
    double sum = 0.0;
    int i;
    for (i = 0; i < d; i++) {
        sum += pow(p1[i] - p2[i], 2);
    }
    return sum;
}

/*
Expected Input:
matrix - an array representing a matrix of size rows x cols
rows, cols - ints representing the dimensions of the matrix
Expected Output:
None, prints the matrix
*/
void print_matrix(double* matrix, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            printf("%.4f", matrix[i * cols + j]);
            if (j < cols - 1) {
                printf(",");
            }
        }
        printf("\n");
    }
}

/*
Expected Input:
diag - an array of size n representing the diagonal elements of a diagonal matrix
n - int representing the number of rows and columns
Expected Output:
None, prints the full n x n diagonal matrix, with diagonal elements from the 'diag' array and zeros elsewhere
*/
void print_diag_matrix(double* diag, int n) {
    int i, j;
    for (i = 0; i < n; i++) {
        j = 0;
        for(j = 0; j < n; j++){
            if (j == i){    
                printf("%.4f", diag[i]);
            }
            else{
                printf("%.4f", 0.0);
            }
            if(j < n-1){
                printf(",");
            }
        }
        printf("\n");
    }
}

/*
Expected Input:
left - an array representing a matrix of size left_rows x shared_dim
right - an array representing a matrix of size shared_dim x right_cols
output - an allocated array of size left_rows x right_cols to store the result
left_rows, right_cols, shared_dim - ints defining the dimensions for matrix multiplication
Expected Output:
None, the output array is filled with the product of the matrices left and right
*/
void mult_matrix(double* left, double* right, double* output, int left_rows, int right_cols, int shared_dim) {
    int i, j, k;
    for (i = 0; i < left_rows; i++) {
        for (j = 0; j < right_cols; j++) {
            output[i * right_cols + j] = 0.0;
            for (k = 0; k < shared_dim; k++) {
                output[i * right_cols + j] += left[i * shared_dim + k] * right[k * right_cols + j];
            }
        }
    }
}

/*
Expected Input:
matrix - an array representing a matrix of size rows x cols
transposed - an allocated array of size cols x rows to store the transposed matrix
rows, cols - ints defining the dimensions of the input matrix
Expected Output:
None, the transposed array is filled with the transposed matrix
*/
void transpose(double* matrix, double* transposed, int rows, int cols) {
    int i, j;
    for (i = 0; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            transposed[j * rows + i] = matrix[i * cols + j];
        }
    }
}

 /*
Expected Input:
sym_matrix - an n x n array representing the similarity matrix A
diag - an array of size n representing the diagonal elements of the degree matrix D
output - an allocated n x n array to store the result (Normalized matrix W)
n - int representing the number of datapoints
Expected Output:
None, the output array is filled with the normalized matrix W
*/
void mult_by_sqrt_of_diag(double* sym_matrix, double* diag, double* output, int n) {
    int i, j;
    double inv_sqrt_d_ii, inv_sqrt_d_jj;
    for (i = 0; i < n; i++) {
        for (j = 0; j < n; j++) {
            output[i * n + j] = sym_matrix[i * n + j] * sqrt(diag[j]);
            inv_sqrt_d_ii = 1.0 / sqrt(diag[i]);
            inv_sqrt_d_jj = 1.0 / sqrt(diag[j]);
            output[i * n + j] = sym_matrix[i * n + j] * inv_sqrt_d_ii * inv_sqrt_d_jj;
        }
    }
}