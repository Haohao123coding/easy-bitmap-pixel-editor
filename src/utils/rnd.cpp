// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <cstdlib>
#include <ctime>
#include <cstdint>

#include "rnd.h"

int32_t rnd::setupRandomSeed(){
    try{
        srand(time(nullptr));
    }catch(...){
        return 1;
    }
    return 0;
}

int32_t rnd::setupRandomSeed(uint32_t seed){
    try{
        srand(seed);
    }catch(...){
        return 1;
    }
    return 0;
}

int32_t rnd::randInt(int32_t l, int32_t r){
    return rand() % (r - l + 1) + l;
}
