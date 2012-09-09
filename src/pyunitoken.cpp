#include <Python.h>
#include <string.h>
#include <UniToken.h>
#include "pyunitoken.h"

#define MAXBYTE          0xff

static PyObject *PyUniTokenError;
static PyObject *PyUniTokenNoFileError;

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
    UT_HANDLE handle = 0;
    unsigned long level = 0;
    unsigned char *pin;
    UT_RV   Result = 0;


    if (!PyArg_ParseTuple(args, "lls", &handle, &level, &pin))
        return NULL;

    if (handle > 0) { 
        Result =  UT_Logon((UT_HANDLE) handle, level, pin);
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

}

static PyObject *
TokenLogout(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    UT_RV   Result;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result = UT_Logoff((UT_HANDLE) handle);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_RuntimeError, err2msg(Result));
             return NULL;
        }
        }
    else {
        PyErr_SetString(PyExc_RuntimeError, (char *) "Handle empty");
        return NULL;
    };
    Py_INCREF(Py_None);
    return Py_None;

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

static PyObject *
ChangePin(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long level = 0;
    unsigned char *pin;
    unsigned char *newpin;
    UT_RV   Result = 0;


    if (!PyArg_ParseTuple(args, "llss", &handle, &level, &pin, &newpin))
        return NULL;

    if (handle > 0) { 
        Result =  UT_ChangePin((UT_HANDLE) handle, level, UT_CHANGE_PIN_MODE_BYSELF, pin, newpin);
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

}

static PyObject *
FormatToken(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    UT_RV   Result = 0;


    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_Format((UT_HANDLE) handle);
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

}

static PyObject *
GetSoftId(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long softid = 0;
    UT_RV   Result = 0;


    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_GetSoftID((UT_HANDLE) handle, &softid);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
        }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", softid);
}

static PyObject *
SetSoftId(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long softid = 0;
    UT_RV   Result = 0;


    if (!PyArg_ParseTuple(args, "ll", &handle, &softid))
        return NULL;

    if (handle > 0) { 
        Result =  UT_SetSoftID((UT_HANDLE) handle, softid);
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
}

static PyObject *
RSAGenKeyPair(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long modulus = 0;
    unsigned long pubexp = 0;
    KEY_HANDLE_PTR pubkeyhandle;
    KEY_HANDLE_PTR prikeyhandle;
    UT_RV   Result = 0;
    PyObject *rslt = PyTuple_New(2);

    if (!PyArg_ParseTuple(args, "lll", &handle, &modulus, &pubexp))
        return NULL;

    if (handle > 0) { 
        Result =  UT_RSAGenerateKeyPair(
            (UT_HANDLE) handle, 
            modulus,
            pubexp, 
            (KEY_HANDLE_PTR) &pubkeyhandle, 
            (KEY_HANDLE_PTR) &prikeyhandle);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    PyTuple_SetItem(rslt, 0, Py_BuildValue("l", pubkeyhandle));
    PyTuple_SetItem(rslt, 1, Py_BuildValue("l", prikeyhandle));
    return rslt;
}

static PyObject *
RSAGetKeyPairCount(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long keycount = 0;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_RSAGetKeyPairCount(
            (UT_HANDLE) handle, 
            &keycount);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };
    
    return Py_BuildValue("l", keycount);
}

static PyObject *
RSAPubKeyEncrypt(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned char *data = NULL;
    unsigned long datalen = 0;
    unsigned char encdata[512];
    unsigned long encdatalen = 0;
    unsigned long keymodulus = 0;
    char errmsg[128];
    UT_HANDLE pubkeyhandle;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "llz#", &handle, &pubkeyhandle, &data, &datalen))
        return NULL;
   
    UT_RSAGetKeyPairModulus(handle, pubkeyhandle, &keymodulus);

    if(datalen != keymodulus) {
        snprintf((char *) &errmsg, 128, "Data length is %lu, key modulus is %lu, they should match", datalen*8, keymodulus*8);
        PyErr_SetString(PyExc_IOError, (char *) &errmsg); 
        return NULL;
        };
    if (handle > 0) { 
        Result =  UT_RSAPubKeyEncrypt(
            (UT_HANDLE) handle, 
            pubkeyhandle,
            data, 
            datalen, 
            encdata,
            &encdatalen);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };
    
    return Py_BuildValue("s#", &encdata, encdatalen);
}

static PyObject *
RSAPriKeyDecrypt(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned char *encdata = NULL;
    unsigned long encdatalen = 0;
    unsigned char data[512];
    unsigned long datalen = 0;
    unsigned long keymodulus = 0;
    char errmsg[128];
    UT_HANDLE prikeyhandle;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "llz#", &handle, &prikeyhandle, &encdata, &encdatalen))
        return NULL;
  
    UT_RSAGetKeyPairModulus(handle, prikeyhandle, &keymodulus);
    
    if(encdatalen != keymodulus) {
        snprintf((char *) &errmsg, 128, "Data length is %lu, key modulus is %lu, they should match", encdatalen*8, keymodulus*8);
        PyErr_SetString(PyExc_IOError, (char *) &errmsg); 
        return NULL;
        };
    if (handle > 0) { 
        Result =  UT_RSAPriKeyDecrypt(
            (UT_HANDLE) handle, 
            prikeyhandle,
            encdata, 
            encdatalen, 
            data,
            &datalen);
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("s#", &data, datalen);
}

static PyObject *
FsGetSpace(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long used = 0;
    unsigned long free = 0;
    UT_RV   Result = 0;
    PyObject *rslt = PyTuple_New(2);

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_FS_GetSpace(
            (UT_HANDLE) handle, 
            &used,
            &free 
            );
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    PyTuple_SetItem(rslt, 0, Py_BuildValue("l", used));
    PyTuple_SetItem(rslt, 1, Py_BuildValue("l", free));
    return rslt;
}

static PyObject *
FsGetFileCount(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned long filecount = 0;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_FS_GetFileCount(
            (UT_HANDLE) handle, 
            &filecount
            );
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", filecount);
}

static PyObject *
FsGetFirstFileName(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    char    filename[17];
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_FS_GetFirstFileName
        (
            (UT_HANDLE) handle, 
            (char *)&filename
            );
        if (Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("s", &filename);
}

static PyObject *
FsGetNextFileName(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    char    filename[17];
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "l", &handle))
        return NULL;

    if (handle > 0) { 
        Result =  UT_FS_GetNextFileName
        (
            (UT_HANDLE) handle, 
            (char *)&filename
            );
        if (Result == UT_FS_LIST_END || Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("s", &filename);
}

static PyObject *
FsCreateFile(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    unsigned char *filename = NULL;
    char fname[17] = { [0 ... 16] = 0};
    unsigned long filename_len = 0;
    unsigned long file_size = 0;
    unsigned long permissions = 0;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "ls#ll", &handle, &filename, &filename_len,
        &file_size, &permissions))
        return NULL;
 
    memcpy(&fname, filename, 16);
    if (handle > 0) { 
        Result =  UT_FS_CreateFile(
            (UT_HANDLE) handle, 
            fname,
            file_size, 
            permissions 
            );
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", Result);
}

static PyObject *
FsDeleteFile(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    char *filename = NULL;
    unsigned long filename_len = 0;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "ls#", &handle, &filename, &filename_len))
        return NULL;
 
    if (handle > 0) { 
        Result =  UT_FS_DeleteFile(
            (UT_HANDLE) handle, 
            filename
            );
        if (Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", Result);
}

static PyObject *
FsWriteFile(PyObject *self, PyObject *args, PyObject *keywds) {
    UT_HANDLE handle = 0;
    unsigned char *write_buffer = NULL;
    unsigned long length = 0;
    unsigned long offset = 0;
    UT_RV   Result = 0;
    static char *kwlist[] = {(char *)"offset"};
    void *reserve;

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "lz#|l", kwlist, &handle, &write_buffer,
        &length, &offset))
        return NULL;

    if (handle > 0) {
        Result =  UT_FS_WriteFile(
            (UT_HANDLE) handle, 
            offset,
            length, 
            (unsigned char *)write_buffer,
            reserve // lpReserve
            );
        UT_FS_CloseFile(handle, &reserve);
        if (Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", Result);
}

static PyObject *
FsReadFile(PyObject *self, PyObject *args, PyObject *keywds) {
    UT_HANDLE handle = 0;
    unsigned char *read_buffer = NULL;
    unsigned long length = 0;
    unsigned long offset = 0;
    UT_RV   Result = 0;
    static char *kwlist[] = {(char *)"offset"};

    if (!PyArg_ParseTupleAndKeywords(args, keywds, "ll|l", kwlist, &handle, &length, &offset))
        return NULL;
 
    read_buffer = (unsigned char *) malloc(length+1);
    bzero(read_buffer, length+1); 

    if (handle > 0) {
        Result =  UT_FS_ReadFile(
            (UT_HANDLE) handle, 
            offset,
            length, 
            read_buffer,
            NULL // lpReserve
            );
        if (Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("z#", read_buffer, length);
}
static PyObject *
FsOpenFile(PyObject *self, PyObject *args) {
    UT_HANDLE handle = 0;
    char *filename = NULL;
    unsigned long filename_len = 0;
    UT_RV   Result = 0;

    if (!PyArg_ParseTuple(args, "ls#", &handle, &filename, &filename_len))
        return NULL;
 
    if (handle > 0) { 
        Result =  UT_FS_OpenFile(
            (UT_HANDLE) handle, 
            filename,
            NULL // lpReserved
            );
        if (Result == UT_FS_NO_FILE) {
            PyErr_SetString(PyUniTokenNoFileError, err2msg(Result));
            return NULL;
        }
        if (Result != UT_OK) {
             PyErr_SetString(PyExc_IOError, err2msg(Result));
             return NULL;
        }
    }
    else {
        PyErr_SetString(PyExc_IOError, (char *) "Handle empty");
        return NULL;
    };

    return Py_BuildValue("l", Result);
}

static PyMethodDef PyUniTokenMethods[] = {
{"InitToken",  InitToken, METH_VARARGS, "Init token"},
{"GetLibraryVersion",  GetLibraryVersion, METH_VARARGS, "Return library version"},
{"CloseToken", CloseToken, METH_VARARGS, "Close token handle"},
{"GetFirmwareVersion", GetFirmwareVersion, METH_VARARGS, "Get token firmware version"},
{"GetUserLevel", GetUserLevel, METH_VARARGS, "Get current token user level"},
{"TokenLogin", TokenLogin, METH_VARARGS, "Login into token, possible usernames 'user' or 'admin'"},
{"TokenLogout", TokenLogout, METH_VARARGS, "Logout from token (back to guest)'"},
{"ChangePin", ChangePin, METH_VARARGS, "Change pin for selected user"},
{"GetSoftId", GetSoftId, METH_VARARGS, "Get token software id"},
{"SetSoftId", SetSoftId, METH_VARARGS, "Set token software id"},
{"FormatToken", FormatToken, METH_VARARGS, "Format token and clear all it content"},
/* RSA */
{"RSAGenKeyPair", RSAGenKeyPair, METH_VARARGS, "Generate new RSA keypair on token"},
{"RSAGetKeyPairCount", RSAGetKeyPairCount, METH_VARARGS, "Get RSA key count"},
{"RSAPubKeyEncrypt", RSAPubKeyEncrypt, METH_VARARGS, "Encrypt data with public key"},
{"RSAPriKeyDecrypt", RSAPriKeyDecrypt, METH_VARARGS, "Decrypt data with private key"},
/* FS */
{"FsGetSpace", FsGetSpace, METH_VARARGS, "Get usage of secure filesystem"},
{"FsGetFileCount", FsGetFileCount, METH_VARARGS, "Get secure filesystem file count"},
{"FsGetFirstFileName", FsGetFirstFileName, METH_VARARGS, "Get secure filesystem first file"},
{"FsGetNextFileName", FsGetNextFileName, METH_VARARGS, "Get secure filesystem file next file"},
{"FsCreateFile", FsCreateFile, METH_VARARGS, "Create file on secure filesystem"},
{"FsOpenFile", FsOpenFile, METH_VARARGS, "Open file on secure filesystem"},
{"FsDeleteFile", FsDeleteFile, METH_VARARGS, "Delete file on secure filesystem"},
{"FsWriteFile", (PyCFunction) FsWriteFile, METH_VARARGS|METH_KEYWORDS, "Write file on secure filesystem"},
{"FsReadFile", (PyCFunction) FsReadFile, METH_VARARGS|METH_KEYWORDS, "Read file on secure filesystem"},
{NULL, NULL,  0, NULL}        /* Sentinel */
};

PyMODINIT_FUNC
initpyunitoken(void)
{
    PyObject *m;
    PyObject *v;
    unsigned int i = 0;

    m = Py_InitModule("pyunitoken", PyUniTokenMethods);
    if (m == NULL) 
        return;
    PyUniTokenError = PyErr_NewException((char *)"pyunitoken.UniTokenError", NULL, NULL);
    PyUniTokenNoFileError = PyErr_NewException((char *)"pyunitoken.UniTokenNoFileError", NULL, NULL);

    Py_INCREF(PyUniTokenError);
    Py_INCREF(PyUniTokenNoFileError);
    
    PyModule_AddObject(m, "UniTokenNoFileError", PyUniTokenNoFileError);
    PyModule_AddObject(m, "UniTokenError", PyUniTokenError);

    while (constants[i].name != NULL) {
        v = PyLong_FromLong(constants[i].value); 
        PyObject_SetAttrString(m, constants[i].name, v);
        Py_DECREF(v);
        i++;
    };
}
