import numpy as np
from matrix_opencl import add_matrix

# Create two test matrices
matrix1 = np.array([[1.0, 2.0, 3], [3.0, 4.0, 5]], dtype=np.float64)
matrix2 = np.array([[5.0, 6.0, 7], [7.0, 8.0, 9]], dtype=np.float64)

# Add them using your OpenCL implementation
result = add_matrix(matrix1, matrix2)

print("Matrix 1:")
print(matrix1)
print("\nMatrix 2:")
print(matrix2)
print("\nResult:")
print(result)