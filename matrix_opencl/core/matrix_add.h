#ifndef MATRIX_ADD_H
#define MATRIX_ADD_H

#ifdef __cplusplus
extern "C"
{
#endif

	void matrix_add(float *input_matrix1, float *input_matrix2, float *output_matrix, int width, int height, int *error);

#ifdef __cplusplus
}
#endif

#endif // MATRIX_ADD_H