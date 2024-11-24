from .numpy_interface import add_matrix

# Version of the core module
__version__ = "0.1.0"

# Public API
__all__ = [
    'add_matrix',
    'MatrixDimError',  # Our custom exception
]

class MatrixDimError(ValueError):
    """Exception raised when matrix dimensions don't match."""
    pass

# Optional: Add any initialization code that needs to run when the core module is imported
def _initialize_opencl():
    """Initialize OpenCL environment if needed."""
    # Any OpenCL initialization code would go here
    pass

# Run initialization when the module is imported
_initialize_opencl()