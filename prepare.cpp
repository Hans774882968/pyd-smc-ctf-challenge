#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include "smc.h"
#define DLLEXPORT extern "C" __declspec(dllexport)
// Copyright 2024 hans774882968

DLLEXPORT PyObject* prepare_const_bytes(PyObject* self, PyObject* args) {
  PyObject* read_lyric_txt = nullptr;
  if (!PyArg_ParseTuple(args, "O", &read_lyric_txt)) {
    PyErr_SetString(PyExc_ValueError, "Parse function read_lyric_txt failed");
    return nullptr;
  }
  if (!PyCallable_Check(read_lyric_txt)) {
    PyErr_SetString(PyExc_ValueError, "read_lyric_txt not callable");
    return nullptr;
  }

  PyObject* read_lyric_txt_args = PyTuple_New(0);
  // PyObject *PyObject_CallNoArgs(PyObject *callable) is added in version 3.9
  PyObject* data_avg_py =
      PyObject_Call(read_lyric_txt, read_lyric_txt_args, nullptr);
  if (!PyLong_Check(data_avg_py)) {
    return nullptr;
  }
  // data_avg = 177
  int data_avg = PyLong_AsLong(data_avg_py);

  const int N = 12;
  int dat[N] = {2, 2, 173, 43, 2, 173, 2, 12, 2, 12, 13, 167};
  char res[N + 1] = {0};
  res[N] = '\0';
  for (size_t i = 0; i < N; i++) {
    res[i] = data_avg % dat[i];
  }
  PyObject* res_py = PyBytes_FromString(res);
  return res_py;
}

static PyMethodDef prepare_methods[] = {
    {"prepare_const_bytes", prepare_const_bytes, METH_VARARGS,
     "generate a constant bytes object of length 12"},
    {NULL, NULL, 0, NULL}  // Sentinel is required
};

static struct PyModuleDef prepare_module = {
    PyModuleDef_HEAD_INIT, "prepare", NULL, -1,
    prepare_methods  // comment to regulate the behavior of clang-format
};

const vector<int> enc_key = {55, 228, 83,  146, 49, 220, 225,
                             87, 135, 171, 147, 50, 87,  250};

PyMODINIT_FUNC PyInit_prepare() {
  change_page((UINT64)(prepare_const_bytes));
  int tmp2 = 114514;
  asm goto(
      ".intel_syntax noprefix\n"
      "mov eax, %0\n"
      "cmp eax, 114514\n"
      "jz %l[label2]\n"
      ".byte 0xEB\n"
      :
      : "r"(tmp2)
      : "%rax"
      : label2);
label2:
  uint8_t* prepare_const_bytes_st =
      reinterpret_cast<uint8_t*>(prepare_const_bytes);
  int prepare_const_bytes_sz = 0x38BB11541 - 0x38BB11370;
  smc_dec(prepare_const_bytes_st, prepare_const_bytes_sz, enc_key);
  return PyModule_Create(&prepare_module);
}
