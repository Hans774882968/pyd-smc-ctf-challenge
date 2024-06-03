#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include <algorithm>
#include "smc.h"
using std::string;
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

DLLEXPORT PyObject* is_legal_uname(PyObject* self, PyObject* args) {
  const char* uname_c = nullptr;
  if (!PyArg_ParseTuple(args, "s", &uname_c)) {
    PyErr_SetString(PyExc_ValueError, "Parse params failed");
    return nullptr;
  }
  string uname = uname_c;
  if (uname.size() > 50) {
    PyErr_SetString(PyExc_ValueError,
                    "len(uname) should be less than or equal to 50");
    return nullptr;
  }
  bool res = all_of(uname.begin(), uname.end(), isprint);
  return PyBool_FromLong(res);
}

DLLEXPORT PyObject* prepare_const_bytes_easy(PyObject* self, PyObject* args) {
  PyObject* read_lyric_txt = nullptr;
  if (!PyArg_ParseTuple(args, "O", &read_lyric_txt)) {
    PyErr_SetString(PyExc_ValueError, "Parse function read_lyric_txt failed");
    return nullptr;
  }
  if (!PyCallable_Check(read_lyric_txt)) {
    PyErr_SetString(PyExc_ValueError, "read_lyric_txt not callable");
    return nullptr;
  }

  const int N = 12;
  char res[N + 1] = {0};
  res[N] = '\0';
  for (size_t i = 0; i < N; i++) {
    res[i] = i + 1;
  }
  PyObject* res_py = PyBytes_FromString(res);
  return res_py;
}

// TODO: 暂时没想到不使用全局数组的做法
const int TMP_SZ = 100;
DWORD64 tmp_x_arr[TMP_SZ];
int tmp_arr_sz = 0;

void x64_inline_hook(DWORD64 qw_target_addr, DWORD64 p_hook_fn_addr) {
  DWORD offset = (DWORD64)&tmp_x_arr[tmp_arr_sz] - qw_target_addr - 6;
  byte hook_inst[6] = {0xff, 0x25};
  *reinterpret_cast<DWORD*>(hook_inst + 2) = offset;
  WriteProcessMemory((LPVOID)-1, (LPVOID)qw_target_addr, hook_inst,
                     sizeof(hook_inst), nullptr);

  byte dest_address_opcode[8] = {0};
  *reinterpret_cast<DWORD64*>(dest_address_opcode) = p_hook_fn_addr;
  WriteProcessMemory((LPVOID)-1, (LPVOID)&tmp_x_arr[tmp_arr_sz],
                     dest_address_opcode, sizeof(dest_address_opcode), nullptr);

  tmp_arr_sz++;
}

static PyMethodDef prepare_methods[] = {
    {"prepare_const_bytes", prepare_const_bytes_easy, METH_VARARGS,
     "generate a constant bytes object of length 12"},
    {"is_legal_uname", is_legal_uname, METH_VARARGS,
     "determines whether a string consists only of printable characters"},
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
  change_page((UINT64)(is_legal_uname));
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
  uint8_t* is_legal_uname_st = reinterpret_cast<uint8_t*>(is_legal_uname);
  int is_legal_uname_sz = 0x38BB116A6 - 0x38BB11542;
  smc_dec(is_legal_uname_st, is_legal_uname_sz, enc_key);

  x64_inline_hook((DWORD64)prepare_const_bytes_easy,
                  (DWORD64)prepare_const_bytes);

  return PyModule_Create(&prepare_module);
}
