// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <iostream>
#include <fstream>
#include "cmdOpr.h"

int main(int argc, char* argv[]){
    bool isScriptFileMode = false;
    std::string openFileName;
    if(argc > 1){
        for(int32_t i = 1; i < argc; i++){
            if(argv[i][0] == '-'){
                std::string argStr = argv[i];
                if(argStr == "--help"){
                    std::cout << "Usage: ebpe [options] [<filename>]" << std::endl;
                    std::cout << "Options:" << std::endl;
                    std::cout << "      --help                     Show help information" << std::endl;
                    std::cout << "  -s  --script <scriptFileName>  Use script." << std::endl;
                    std::cout << "      --version                  Show version information" << std::endl;
                    return 0;
                }else if(argStr == "--version"){
                    std::cout << "Easy Bitmap Pixel Editor (EBPE) v0.1.0" << std::endl;
                    return 0;
                }else if(argStr == "-s" || argStr == "--script"){
                    if(i == argc - 1){
                        std::cerr << "Argument format error!" << std::endl;
                        return 1;
                    }
                    isScriptFileMode = true;
                    std::string scriptFileName = argv[i+1];
                    freopen(scriptFileName.c_str(), "r", stdin);
                    i++;
                }else{
                    std::cerr << "Unknown argument!" << std::endl;
                    return 1;
                }
            }else{
                openFileName = argv[i];
            }
        }
    }
    if(!openFileName.empty()){
        curFile.openBMP(openFileName, isScriptFileMode);
    }
    while(true){
        cmdOpr::loopTime(isScriptFileMode);
    }
    return 0;
}
