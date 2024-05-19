#include "smc.h"
// Copyright 2024 hans774882968

void change_page(UINT64 addr) {
  DWORD old;
  SYSTEM_INFO si;
  GetSystemInfo(&si);
  int page_size = si.dwPageSize;
  addr -= addr % page_size;

  if (!VirtualProtect((PVOID)addr, page_size, PAGE_EXECUTE_READWRITE, &old)) {
    printf("Error: %x\n", GetLastError());
  }
}

void smc_dec(uint8_t* st, int sz, const vector<int>& enc_key) {
  int ek_sz = enc_key.size();
  uint8_t* ed = st + sz;
  for (uint8_t* i = st; i <= ed; ++i) *i ^= enc_key[(i - st) % ek_sz];
}
