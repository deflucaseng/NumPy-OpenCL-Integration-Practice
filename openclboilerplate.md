## Required Boilerplate

# Includes

#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>



# Getting Platform and Device
    cl_int err;
    cl_platform_id platform;
    cl_uint num_platforms;
    err = clGetPlatformIDs(1, &platform, &num_platforms);
    CHECK_ERROR(err);

    // Get device
    cl_device_id device;
    cl_uint num_devices;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
    CHECK_ERROR(err)

# Creating context and command queue

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    CHECK_ERROR(err);

	cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    CHECK_ERROR(err);


# Creating Memory Buffers


    cl_mem bufferA = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  SIZE * SIZE * sizeof(float), A, &err);
    CHECK_ERROR(err);

    cl_mem bufferB = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                  SIZE * SIZE * sizeof(float), B, &err);
    CHECK_ERROR(err);

    cl_mem bufferC = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                  SIZE * SIZE * sizeof(float), NULL, &err);
    CHECK_ERROR(err);



# Create and build the kernel program
    const char *kernel_source = "\n"
    "__kernel void matrixMultiply(\n"
    "   __global const float* A,\n"
    "   __global const float* B,\n"
    "   __global float* C,\n"
    "   const int SIZE) {\n"
    "    int row = get_global_id(0);\n"
    "    int col = get_global_id(1);\n"
    "    \n"
    "    float sum = 0.0f;\n"
    "    for (int i = 0; i < SIZE; i++) {\n"
    "        sum += A[row * SIZE + i] * B[i * SIZE + col];\n"
    "    }\n"
    "    C[row * SIZE + col] = sum;\n"
    "}\n";

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, &err);
    CHECK_ERROR(err);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        // If there are build errors, get the error log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char*)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build Error: %s\n", log);
        free(log);
        exit(1);
    }


# Create the kernel

    cl_kernel kernel = clCreateKernel(program, "matrixMultiply", &err);
    CHECK_ERROR(err);

# Set Kernel Arguments

    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void*)&bufferA);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void*)&bufferB);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void*)&bufferC);
    err |= clSetKernelArg(kernel, 3, sizeof(int), (void*)&SIZE);
    CHECK_ERROR(err);

# Execute the Kernel
    size_t global_work_size[2] = {SIZE, SIZE};
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, NULL, 
                                0, NULL, NULL);
    CHECK_ERROR(err);

# Wait for commands to complete

 clFinish(queue);

# Read back the results:
    err = clEnqueueReadBuffer(queue, bufferC, CL_TRUE, 0, SIZE * SIZE * sizeof(float),
                             C, 0, NULL, NULL);
    CHECK_ERROR(err);


# Accessing results:

    printf("First element of result: %f\n", C[0]);

# Clean Up:

    clReleaseMemObject(bufferA);
    clReleaseMemObject(bufferB);
    clReleaseMemObject(bufferC);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);
    
    free(A);
    free(B);
    free(C);

    return 0;