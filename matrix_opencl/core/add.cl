__kernel void matrixAdd(
   __global const float* A,
   __global const float* B,
   __global float* C,
   const int WIDTH)
{
    int row = get_global_id(1);
    int col = get_global_id(0);
    int index = row * WIDTH + col;
    C[index] = A[index] + B[index];
}