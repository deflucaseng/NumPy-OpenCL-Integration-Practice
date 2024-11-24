from setuptools import setup, Extension, find_packages
import numpy as np

ext_modules = [
    Extension(
        "matrix_opencl.core.numpy_interface",
        sources=[
            "matrix_opencl/core/numpy_interface.cpp",
            "matrix_opencl/core/matrix_add.c"
        ],
        include_dirs=[
            np.get_include(),
            "/usr/include/CL",
            "matrix_opencl/core"
        ],
        libraries=['OpenCL'],
        library_dirs=['/usr/lib/x86_64-linux-gnu'],
        extra_compile_args=[
            '-std=c++11',
            '-Wno-deprecated-declarations',
            '-Wno-unused-function',
            '-O3'
        ],
        extra_link_args=['-lOpenCL'],
        language='c++'
    )
]

setup(
    name="matrix_opencl",
    packages=find_packages(),
    ext_modules=ext_modules,
    install_requires=[
        'numpy>=2.0.0',  # Specify minimum NumPy version
    ],
    python_requires='>=3.7',
)