// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#ifndef EASY_BITMAP_PIXEL_EDITOR_UTILS_H
#define EASY_BITMAP_PIXEL_EDITOR_UTILS_H

#include <string>
#include "bmpFile.h"

namespace utils{
    int32_t hexToDec(char hex);
    int32_t hex2ToDec(std::string hex);
    color analyseColor(std::string str);
    int32_t stringToUint(std::string str);
}

#endif //EASY_BITMAP_PIXEL_EDITOR_UTILS_H
