#define PY_SSIZE_T_CLEAN
#include <Python.h>

static PyModuleDef moduledef = {
    PyModuleDef_HEAD_INIT, "_ext", NULL, -1, NULL
};

PyMODINIT_FUNC PyInit__ext(void) {
    return PyModule_Create(&moduledef);
}
