#define PY_SSIZE_T_CLEAN
#include <Python.h>

#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
#define PY_ARRAY_UNIQUE_SYMBOL matrix_opencl_ARRAY_API
#include <numpy/ndarrayobject.h>
#include <numpy/ndarraytypes.h>
#include "matrix_add.h"
#include <iostream>

static PyObject *add_matrix(PyObject *self, PyObject *args)
{
    PyArrayObject *input_array1;
    PyArrayObject *input_array2;

    // Parse both input arrays at once
    if (!PyArg_ParseTuple(args, "O!O!", &PyArray_Type, &input_array1,
                          &PyArray_Type, &input_array2))
    {
        return NULL;
    }

    // Check array types
    if (PyArray_TYPE(input_array1) != NPY_FLOAT32 || PyArray_TYPE(input_array2) != NPY_FLOAT32)
    {
        PyErr_SetString(PyExc_TypeError, "Arrays must be of type float32");
        return NULL;
    }

    // Ensure arrays are contiguous
    PyArrayObject *input_array1_cont = (PyArrayObject *)PyArray_ContiguousFromAny(
        (PyObject *)input_array1, NPY_FLOAT32, 2, 2);
    PyArrayObject *input_array2_cont = (PyArrayObject *)PyArray_ContiguousFromAny(
        (PyObject *)input_array2, NPY_FLOAT32, 2, 2);

    if (input_array1_cont == NULL || input_array2_cont == NULL)
    {
        Py_XDECREF(input_array1_cont);
        Py_XDECREF(input_array2_cont);
        return NULL;
    }

    // Get array dimensions
    int nd1 = PyArray_NDIM(input_array1_cont);
    int nd2 = PyArray_NDIM(input_array2_cont);

    if (nd1 != nd2)
    {
        PyErr_SetString(PyExc_ValueError, "Arrays must have same number of dimensions");
        Py_DECREF(input_array1_cont);
        Py_DECREF(input_array2_cont);
        return NULL;
    }

    // Check dimension sizes match
    npy_intp *dims1 = PyArray_DIMS(input_array1_cont);
    npy_intp *dims2 = PyArray_DIMS(input_array2_cont);

    for (int i = 0; i < nd1; i++)
    {
        if (dims1[i] != dims2[i])
        {
            PyErr_SetString(PyExc_ValueError, "Arrays must have matching dimensions");
            Py_DECREF(input_array1_cont);
            Py_DECREF(input_array2_cont);
            return NULL;
        }
    }

    // Calculate total size
    npy_intp total_size = PyArray_SIZE(input_array1_cont);

    // Create output array
    PyArrayObject *output_array = (PyArrayObject *)PyArray_SimpleNew(nd1, dims1, NPY_FLOAT32);   
    if (output_array == NULL)
    {
        Py_DECREF(input_array1_cont);
        Py_DECREF(input_array2_cont);
        return NULL;
    }

    // Get data pointers
    float *input_data1 = (float *)PyArray_DATA(input_array1_cont);
    float *input_data2 = (float *)PyArray_DATA(input_array2_cont);
    float *output_data = (float *)PyArray_DATA(output_array);

    // Call matrix addition
    int err = 0;
    matrix_add(input_data1, input_data2, output_data, dims2[1], dims2[0], &err);

    if (err)
    {
        Py_DECREF(input_array1_cont);
        Py_DECREF(input_array2_cont);
        Py_DECREF(output_array);
        PyErr_SetString(PyExc_RuntimeError, "Matrix addition failed");
        return NULL; // Return NULL to indicate an error
    }



    return (PyObject *)output_array;
}

static PyMethodDef ModuleMethods[] = {
    {"add_matrix", add_matrix, METH_VARARGS, "Add two matrices using OpenCL"},
    {NULL, NULL, 0, NULL}};

static struct PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT,
    "numpy_interface",
    "Module for OpenCL-accelerated matrix operations",
    -1,
    ModuleMethods};

PyMODINIT_FUNC PyInit_numpy_interface(void)
{
    // Initialize NumPy
    import_array();
    if (PyErr_Occurred())
    {
        return NULL;
    }

    // Create module
    PyObject *module = PyModule_Create(&moduledef);
    if (module == NULL)
    {
        return NULL;
    }

    return module;
}