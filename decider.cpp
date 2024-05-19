#define PY_SSIZE_T_CLEAN
#include <Python.h>
#include <string>
#include "smc.h"
using std::string;
#define DLLEXPORT extern "C" __declspec(dllexport)
// Copyright 2024 hans774882968

DLLEXPORT bool jdg_part1(const char* uname, string passwd_part1,
                         PyObject* get_hash) {
  if (passwd_part1.size() != 16) {
    return false;
  }
  PyObject* get_hash_args = PyTuple_New(1);
  PyTuple_SetItem(get_hash_args, 0, Py_BuildValue("s", uname));
  PyObject* uname_hash_py = PyObject_Call(get_hash, get_hash_args, nullptr);

  const char* uname_hash_c = PyUnicode_AsUTF8AndSize(uname_hash_py, nullptr);
  if (uname_hash_c == nullptr) {
    return false;
  }
  string uname_hash_16(uname_hash_c, 16);
  if (passwd_part1 != uname_hash_16) {
    return false;
  }
  return true;
}

DLLEXPORT bool jdg_part2(const char* uname, string passwd_part2,
                         const char* jdg_part2_bytes) {
  const int bl = strlen(jdg_part2_bytes);
  int sl = strlen(uname);
  if (passwd_part2.size() != sl) {
    return false;
  }
  for (size_t i = 0; i < sl; i++) {
    int val = uname[i] ^ passwd_part2[i];
    if (jdg_part2_bytes[i % bl] != val) {
      return false;
    }
  }
  return true;
}

DLLEXPORT PyObject* jdg(PyObject* self, PyObject* args) {
  const char* uname_c = nullptr;
  const char* passwd_c = nullptr;
  PyObject* get_hash = nullptr;
  const char* jdg_part2_bytes = nullptr;
  // https://docs.python.org/3/c-api/arg.html
  // jdg_part2_bytes 里不能有 \x00 否则报错
  if (!PyArg_ParseTuple(args, "ssOy", &uname_c, &passwd_c, &get_hash,
                        &jdg_part2_bytes)) {
    PyErr_SetString(PyExc_ValueError, "Parse params failed");
    return nullptr;
  }
  if (!PyCallable_Check(get_hash)) {
    PyErr_SetString(PyExc_ValueError, "get_hash not callable");
    return nullptr;
  }
  string passwd = passwd_c;
  int separator_idx = passwd.find_first_of("-");
  string passwd_part1 = passwd.substr(0, separator_idx);
  string passwd_part2 = passwd.substr(separator_idx + 1);

  if (!jdg_part1(uname_c, passwd_part1, get_hash)) {
    return Py_False;
  }
  if (!jdg_part2(uname_c, passwd_part2, jdg_part2_bytes)) {
    return Py_False;
  }
  return Py_True;
}

static PyMethodDef decider_methods[] = {
    {"jdg", jdg, METH_VARARGS, "judge user name and password"},
    {NULL, NULL, 0, NULL}  // Sentinel is required
};

static struct PyModuleDef decider_module = {
    PyModuleDef_HEAD_INIT, "decider", NULL, -1,
    decider_methods  // comment to regulate the behavior of clang-format
};

const vector<int> enc_key = {67, 91,  48, 88, 80, 75, 41,
                             93, 116, 92, 33, 83, 68, 69};

PyMODINIT_FUNC PyInit_decider() {
  change_page((UINT64)(jdg_part1));
  change_page((UINT64)(jdg_part2));
  change_page((UINT64)(jdg));
  asm goto(
      ".intel_syntax noprefix\n"
      "xor rax, rax\n"
      "jz %l[label]\n"
      ".byte 0xEB\n"
      :
      :
      : "%rax"
      : label);
label:
  uint8_t* jdg_part1_st = reinterpret_cast<uint8_t*>(jdg_part1);
  int jdg_part1_sz = 0x321E414B9 - 0x321E41370;
  smc_dec(jdg_part1_st, jdg_part1_sz, enc_key);
  uint8_t* jdg_part2_st = reinterpret_cast<uint8_t*>(jdg_part2);
  int jdg_part2_sz = 0x321E4158D - 0x321E414BA;
  smc_dec(jdg_part2_st, jdg_part2_sz, enc_key);
  uint8_t* jdg_st = reinterpret_cast<uint8_t*>(jdg);
  int jdg_sz = 0x321E4184B - 0x321E4158E;
  smc_dec(jdg_st, jdg_sz, enc_key);
  return PyModule_Create(&decider_module);
}
