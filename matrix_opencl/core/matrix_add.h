#ifndef MATRIX_ADD_H
#define MATRIX_ADD_H

#ifdef __cplusplus
extern "C" {
#endif

void matrix_add(double* input_matrix1, int width1, int height1, double* input_matrix2, int width2, int height2, double* output_matrix, int* error);

#ifdef __cplusplus
}
#endif

#endif // MATRIX_ADD_H