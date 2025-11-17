// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include "utils.h"

int32_t utils::hexToDec(char hex){
    if(hex >= '0' && hex <= '9'){
        return hex - '0';
    }else if(hex >= 'A' && hex <= 'F'){
        return hex - 'A' + 10;
    }else if(hex >= 'a' && hex <= 'f'){
        return hex - 'a' + 10;
    }else{
        return 0;
    }
}

int32_t utils::hex2ToDec(std::string hex){
    return utils::hexToDec(hex[0]) * 16 + utils::hexToDec(hex[1]);
}

color utils::analyseColor(std::string str){
    int32_t colorByte;
    uint32_t len = str.length();
    if(len == 7){
        colorByte = 3;
    }else{
        colorByte = 4;
    }
    if(str[0] != '#' || (len != 7 && len != 9)){
        // wrong format
        return colorByte == 3 ? color(0, 0, 0) : color(0, 0, 0, 0);
    }
    for(int32_t i = 1; i < len; i++){
        if(!((str[i] >= '0' && str[i] <= '9') ||
        (str[i] >= 'a' && str[i] <= 'f'))){
            // wrong format
            return colorByte == 3 ? color(0, 0, 0) : color(0, 0, 0, 0);
        }
    }
    uint8_t r = static_cast<uint8_t>(hex2ToDec(str.substr(1, 2)));
    uint8_t g = static_cast<uint8_t>(hex2ToDec(str.substr(3, 2)));
    uint8_t b = static_cast<uint8_t>(hex2ToDec(str.substr(5, 2)));
    uint8_t a = 0;
    if(colorByte == 4) a = static_cast<uint8_t>(hex2ToDec(str.substr(7, 2)));
    if(colorByte == 3){
        return {r, g, b};
    }else{
        return {r, g, b, a};
    }
}

int32_t stringToInt(const std::string& str){
    uint32_t len = str.length();
    for(uint32_t i = 0; i < len; i++){
        if(!isdigit(str[i])){
            return -1;
        }
    }
    return std::stoi(str);
}
