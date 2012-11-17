#include <Python.h>
#include <UniToken.h>
#include "pyunitoken.h"

#define MAXBYTE          0xff

static PyObject *PyUniTokenError;

char *
err2msg(UT_RV errcode)
{
    for (int i = 0; error_codes[i].name; ++i)
        if (error_codes[i].value == errcode)
            return error_codes[i].name;
    return (char *) "unknown error";
}

static PyObject *
InitToken(PyObject *self, PyObject *args) {
    UT_RV           Result = ~UT_OK;
    UT_HANDLE       Handle = 0;
    UT_SLOT_ID     *lpSlotID = NULL;
    unsigned long   TokenCount = 0;

    Result = UT_Initialize();
    if (Result != UT_OK) {
        if (Result != UT_ALREADY_INITIALIZE) {
            PyErr_SetString(PyExc_RuntimeError, err2msg(Result));
            return NULL;
        }
            printf("Warning: token already initialized\n");
        }
        Result = ~UT_OK;
    Result = UT_GetTokenList(lpSlotID, &TokenCount);
    if (Result != UT_BUFFER_TOO_SMALL) {
        PyErr_SetString(PyExc_IOError, err2msg(Result));
        return NULL;
    }
    Result = ~UT_OK;
    lpSlotID = (UT_SLOT_ID *) malloc(TokenCount * sizeof(UT_SLOT_ID));
    Result = UT_GetTokenList(lpSlotID, &TokenCount);
    if (Result != UT_OK) {
        PyErr_SetString(PyExc_IOError, err2msg(Result));
         return NULL;
    }
    Result = ~UT_OK;

    Result = UT_OpenDevice(lpSlotID[0], &Handle);
    if (Result != UT_OK) {
         PyErr_SetString(PyExc_IOError, err2msg(Result));
         return NULL;
    } else {
        return Py_BuildValue("l", Handle);
    }
}

static PyObject *
GetLibraryVersion(PyObject *self, PyObject *args) {
    unsigned long library_version;

    library_version = UT_GetLibraryVersion();

    if (library_version) {
        return Py_BuildValue("l", library_version);;
        }
    else {
        Py_INCREF(Py_None);
        return Py_None;
    }
};

static PyObject *
CloseToken(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    UT_RV   Result;


    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result = UT_CloseDevice((UT_HANDLE) handle);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
        }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };
        
    Py_INCREF(Py_None);
    return Py_None;
};

static PyObject *
GetFirmwareVersion(PyObject *self, PyObject *args) {
    unsigned long fw_version = 0;
    UT_HANDLE handle = 0;
    UT_RV   Result;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result = UT_GetFirmwareVersion((UT_HANDLE) handle, &fw_version);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_RuntimeError, err2msg(Result));
             return NULL;
        }
        }
    else {
        PyErr_SetString(PyExc_RuntimeError, (char *) "Handle empty");
        return NULL;
    };
    
    return Py_BuildValue("l", fw_version);
};

static PyObject *
TokenLogin(PyObject *self, PyObject *args) {
}

static PyObject *
TokenLogout(PyObject *self, PyObject *args) {
}

static PyObject *
GetUserLevel(PyObject *self, PyObject *args) {
    unsigned long level = 0;
    char *level_name;
    UT_HANDLE handle = 0;
    UT_RV   Result;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result = UT_GetCurrentUserLevel((UT_HANDLE) handle, &level);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_RuntimeError, err2msg(Result));
             return NULL;
        }
        }
    else {
        PyErr_SetString(PyExc_RuntimeError, (char *) "Handle empty");
        return NULL;
    };
   
    printf("level %lu\n", level);
    switch(level) {
        case UT_USER_LEVEL_GUEST:
                {level_name = (char *)"guest";break;}
        case UT_USER_LEVEL_USER:
                {level_name = (char *)"user";break;}
        case UT_USER_LEVEL_ADMIN:
                {level_name = (char *)"admin";break;}
        default:
                level_name = (char *)"unknown";
    }
    return Py_BuildValue("s", level_name);

}

static PyMethodDef PyUniTokenMethods[] = {
{"InitToken",  InitToken, METH_VARARGS, "Init token"},
{"GetLibraryVersion",  GetLibraryVersion, METH_VARARGS, "Return library version"},
{"CloseToken", CloseToken, METH_VARARGS, "Close token handle"},
{"GetFirmwareVersion", GetFirmwareVersion, METH_VARARGS, "Get token firmware version"},
{"GetUserLevel", GetUserLevel, METH_VARARGS, "Get current token user level"},
{NULL, NULL,  0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initpyunitoken(void)
{
    PyObject *m;

    m = Py_InitModule("pyunitoken", PyUniTokenMethods);
    if (m == NULL) 
        return;
    PyUniTokenError = PyErr_NewException((char *)"unitoken.UniTokenError", NULL, NULL);
    Py_INCREF(PyUniTokenError);
    PyModule_AddObject(m, "UniTokenError", PyUniTokenError);
}
