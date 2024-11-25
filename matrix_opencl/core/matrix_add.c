#include <CL/cl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SOURCE_SIZE (0x100000)

#define CHECK_ERROR(err, error_ptr)                                           \
    if (err != CL_SUCCESS)                                                   \
    {                                                                        \
        printf("[ERROR] Failed at line %d with error: %d\n", __LINE__, err); \
        *(error_ptr) = 1;                                                    \
        return;                                                             \
    }

char *readKernelSource(const char *filename)
{
    FILE *file = fopen("matrix_opencl/core/add.cl", "r");
    if (!file)
    {
        printf("Failed to open kernel file\n");
        exit(1);
    }

    // Get file size
    fseek(file, 0, SEEK_END);
    size_t size = ftell(file);
    rewind(file);

    // Allocate memory for the source code (plus 1 for null terminator)
    char *source = (char *)malloc(size + 1);
    if (!source)
    {
        printf("Failed to allocate memory for kernel source\n");
        fclose(file);
        exit(1);
    }

    // Read the source code
    size_t read_size = fread(source, 1, size, file);
    fclose(file);

    // Add null terminator
    source[read_size] = '\0';

    return source;
}

void matrix_add(float *input_matrix1, float *input_matrix2, float *output_matrix, int width, int height, int *error){
    cl_int err;

    cl_platform_id platform;
    cl_uint num_platforms;
    err = clGetPlatformIDs(1, &platform, &num_platforms);
    CHECK_ERROR(err, error);

    // Get device
    cl_device_id device;
    cl_uint num_devices;
    err = clGetDeviceIDs(platform, CL_DEVICE_TYPE_GPU, 1, &device, &num_devices);
    CHECK_ERROR(err, error)

    cl_context context = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    CHECK_ERROR(err, error);

    cl_command_queue queue = clCreateCommandQueue(context, device, 0, &err);
    CHECK_ERROR(err, error);

    // Create Memory Buffers
    float *float_input1 = (float *)malloc(width * height * sizeof(float));
    float *float_input2 = (float *)malloc(width * height * sizeof(float));
    float *float_output = (float *)malloc(width * height * sizeof(float));

    // Convert floats to floats
    for (int i = 0; i < width * height; i++)
    {
        float_input1[i] = (float)input_matrix1[i];
        float_input2[i] = (float)input_matrix2[i];
    }

    cl_mem input_array_1 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                          width * height * sizeof(float), float_input1, &err);
    cl_mem input_array_2 = clCreateBuffer(context, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR,
                                          width * height * sizeof(float), float_input2, &err);
    cl_mem output_array = clCreateBuffer(context, CL_MEM_WRITE_ONLY,
                                         width * height * sizeof(float), NULL, &err);

    const char *kernel_source = readKernelSource("add.cl");

    cl_program program = clCreateProgramWithSource(context, 1, &kernel_source, NULL, &err);
    CHECK_ERROR(err, error);

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    
    if (err != CL_SUCCESS)
    {
        // If there are build errors, get the error log
        size_t log_size;
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, 0, NULL, &log_size);
        char *log = (char *)malloc(log_size);
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, log_size, log, NULL);
        printf("Build Error: %s\n", log);
        free(log);
        exit(1);
    }

    cl_kernel kernel = clCreateKernel(program, "matrixAdd", &err);
    CHECK_ERROR(err, error);

    int area = width * height;
    err = clSetKernelArg(kernel, 0, sizeof(cl_mem), (void *)&input_array_1);
    err |= clSetKernelArg(kernel, 1, sizeof(cl_mem), (void *)&input_array_2);
    err |= clSetKernelArg(kernel, 2, sizeof(cl_mem), (void *)&output_array);
    err = clSetKernelArg(kernel, 3, sizeof(int), (void *)&width); // Pass width instead of area
    CHECK_ERROR(err, error);

    size_t global_work_size[2] = {width, height};
    err = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_work_size, NULL,
                                 0, NULL, NULL); // Set work_dim to 2
    CHECK_ERROR(err, error);

    clFinish(queue);

    err = clEnqueueReadBuffer(queue, output_array, CL_TRUE, 0,
                              width * height * sizeof(float),
                              float_output, 0, NULL, NULL);

    for (int i = 0; i < width * height; i++)
    {
        output_matrix[i] = (float)float_output[i];
    }

    // Free temporary arrays
    free(float_input1);
    free(float_input2);
    free(float_output);

    CHECK_ERROR(err, error);

    clReleaseMemObject(input_array_1);
    clReleaseMemObject(input_array_2);
    clReleaseMemObject(output_array);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(context);


    free((void *)kernel_source);
}