#include <string.h>
#include <stdlib.h>
#include "utils.h"


int main(int argc, char **argv)
{
    char* goal;
    int d, n;
    double* datapoints;
    double* sym_matrix;
    double* output_matrix;
    if (argc != 3) {
        printf("An Error Has Occurred\n");
        exit(1);
    }
    goal = argv[1];
    datapoints = read_file_env(argv[2], &n, &d);
    sym_matrix = compute_sym(datapoints, n, d);
    free(datapoints);
    if(strcmp(goal, "sym") == 0){
        print_matrix(sym_matrix, n, n);
    }
    else if(strcmp(goal, "ddg") == 0){
        output_matrix = compute_ddg(sym_matrix, n);
        print_diag_matrix(output_matrix, n);
        free(output_matrix);
    }
    else if(strcmp(goal, "norm") == 0){
        output_matrix = compute_norm(sym_matrix, n);
        print_matrix(output_matrix, n, n);
        free(output_matrix);
    }
    else{
        printf("An Error Has Occurred\n");
        free(sym_matrix);
        exit(1);
    }
    free(sym_matrix);
    return 0;
}

