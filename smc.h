#include <windows.h>
#include <cstdio>
#include <cstdint>
#include <vector>
using std::vector;
// Copyright 2024 hans774882968

#ifndef D__HELLOWORLD_CPP_411_PYD_SMC_CTF_CHALLENGE_SMC_H_
#define D__HELLOWORLD_CPP_411_PYD_SMC_CTF_CHALLENGE_SMC_H_

void change_page(UINT64 addr);

void smc_dec(uint8_t* st, int sz, const vector<int>& enc_key);

#endif  // D__HELLOWORLD_CPP_411_PYD_SMC_CTF_CHALLENGE_SMC_H_
