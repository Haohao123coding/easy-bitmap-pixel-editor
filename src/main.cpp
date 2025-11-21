// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <iostream>
#include <fstream>
#include "cmdOpr.h"

int main(int argc, char* argv[]){
    bool isScriptFileMode = false;
    if(argc != 1 && argc != 2){
        std::cerr << "Argument count error!" << std::endl;
        return 1;
    }
    if(argc == 2){
        isScriptFileMode = true;
        std::string scriptFileName = argv[1];
        freopen(scriptFileName.c_str(), "r", stdin);
    }
    while(true){
        cmdOpr::loopTime(isScriptFileMode);
    }
    return 0;
}
