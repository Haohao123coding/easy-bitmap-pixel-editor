// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#ifndef EASY_BITMAP_PIXEL_EDITOR_RAND_H
#define EASY_BITMAP_PIXEL_EDITOR_RAND_H

#include <cstdint>

namespace rnd{
    int32_t setupRandomSeed();
    int32_t setupRandomSeed(uint32_t seed);
    int32_t randInt(int32_t l, int32_t r); // [l, r)
}

#endif //EASY_BITMAP_PIXEL_EDITOR_RAND_H
