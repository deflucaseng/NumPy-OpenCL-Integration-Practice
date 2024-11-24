### Notes


## Required Boilerplate:

# Function Support

static PyObject* funcname(PyObject* self, PyObject* args){
	PyArg_ParseTuple(....)



	Py_DECREF(....) For cleaning up

	PyArray_Return(output_array)
}



# Method Definition


static PyMethodDef ModuleMethods[] = {
	{"actual_func_name", actual_func_name, METH_VARARGS/METH_NOARGS, "Commenting"}
	{NULL, NULL, 0, NULL}
}

# Module Definition Structure

static struct PyModuleDef moduledef = {
	PyModuleDef_HEAD_INIT,        
	"numpy_interface",    <== Module Name
	"commenting",
	-1,                <== Module State
	ModuleMethods             <== List of Methods
}

# Module Initialization Function

PyMODINIT_FUNC PyINIT_numpy_interface(void){
	import_array();                       <== Initializes NumPy array system
	return PyModule_Create(&moduledef);      <== Creates and returns the module object
}