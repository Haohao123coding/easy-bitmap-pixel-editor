// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <chrono>
#include <cstdint>

#include "rnd.h"

rnd::rnd() : gen(std::chrono::system_clock::now().time_since_epoch().count()){}

int32_t rnd::setupRandomSeed(){
    try{
        auto seed = std::chrono::system_clock::now().time_since_epoch().count();
        gen.seed(seed);
    }catch(...){
        return 1;
    }
    return 0;
}

int32_t rnd::setupRandomSeed(uint32_t seed){
    try{
        gen.seed(seed);
    }catch(...){
        return 1;
    }
    return 0;
}

int32_t rnd::randInt(int32_t l, int32_t r){
    try{
        if(l > r){
            std::swap(l, r);
        }
        std::uniform_int_distribution<int32_t> dist(l, r - 1);
        return dist(gen);
    }catch(...){
        return l;
    }
}
