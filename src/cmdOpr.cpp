// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <iostream>
#include <fstream>

#include "bmpFile.h"
#include "cmdOpr.h"
#include "utils.h"
#include "helpCommand/helpCommand.h"

bmpFile curFile;

int32_t cmdOpr::analyseGen(uint32_t wordCount, const std::vector<std::string>& cmds){
    if(wordCount != 4 && wordCount != 5){
        return 10;
    }
    const std::string& fileName = cmds[1];
    const std::string& widthStr = cmds[2];
    const std::string& heightStr = cmds[3];
    int32_t width = utils::stringToUint(widthStr);
    int32_t height = utils::stringToUint(heightStr);
    if(width == -1 || height == -1){
        return 13;
    }
    color filling;
    if(wordCount == 5){
        filling = utils::analyseColor(cmds[4]);
    }else{
        filling = color(255, 255, 255, 255);
    }
    curFile = bmpFile(width, height, filling, fileName);
    return 0;
}

int32_t cmdOpr::analyseOpen(uint32_t wordCount, const std::vector<std::string>& cmds){
    if(wordCount != 2){
        return 10;
    }
    curFile.openBMP(cmds[1]);
    return 0;
}

int32_t cmdOpr::analyseSave(uint32_t wordCount){
    if(wordCount != 1){
        return 10;
    }
    curFile.saveBMP();
    return 0;
}

int32_t cmdOpr::analyseSet(uint32_t wordCount, const std::vector<std::string>& cmds){
    if(wordCount != 3){
        return 10;
    }
    infoHeader ih = curFile.getInfoHeader();
    int32_t setVal = utils::stringToUint(cmds[2]);
    if(setVal == -1){
        return 13;
    }
    if(cmds[1] == "width"){
        ih.biWidth = setVal;
    }else if(cmds[1] == "height"){
        ih.biHeight = setVal;
    }else{
        return 11;
    }
    curFile.setInfoHeader(ih);
    curFile.calcSetBMP();
    return 0;
}

int32_t cmdOpr::analyseGet(uint32_t wordCount, const std::vector<std::string>& cmds, bool isScriptFileMode){
    if(wordCount != 2){
        return 10;
    }
    int32_t answer = 0;
    if(cmds[1] == "width"){
        answer = curFile.getInfoHeader().biWidth;
    }else if(cmds[1] == "height"){
        answer = curFile.getInfoHeader().biHeight;
    }else{
        return 11;
    }
    if(!isScriptFileMode){
        std::cout << cmds[1] << ": " << answer << std::endl;
    }
    return 0;
}

int32_t cmdOpr::analyseDraw(uint32_t wordCount, const std::vector<std::string>& cmds){
    if(wordCount < 2){
        return 10;
    }
    if(cmds[1] == "pixel"){
        if(wordCount != 5){
            return 10;
        }
        int32_t x = utils::stringToUint(cmds[2]);
        int32_t y = utils::stringToUint(cmds[3]);
        color color_set = utils::analyseColor(cmds[4]);
        if(x < 0 || x >= curFile.getInfoHeader().biHeight ||
        y < 0 || y >= curFile.getInfoHeader().biWidth){
            return 12;
        }
        curFile.editPixel(x, y, color_set);
        return 0;
    }else if(cmds[1] == "rect"){
        if(wordCount != 7 && wordCount != 8){
            return 10;
        }
        int32_t xf = utils::stringToUint(cmds[2]);
        int32_t yf = utils::stringToUint(cmds[3]);
        int32_t xl = utils::stringToUint(cmds[4]);
        int32_t yl = utils::stringToUint(cmds[5]);
        color color_set = utils::analyseColor(cmds[6]);
        if(xf < 0 || xf >= curFile.getInfoHeader().biHeight ||
        xl < 0 || xl >= curFile.getInfoHeader().biHeight ||
        yf < 0 || yf >= curFile.getInfoHeader().biWidth ||
        yl < 0 || yl >= curFile.getInfoHeader().biWidth){
            return 12;
        }
        if(wordCount == 7){
            curFile.drawRect(xf, yf, xl, yl, color_set);
        }else{
            int32_t borderPixelCount = utils::stringToUint(cmds[7]);
            if(borderPixelCount == -1){
                return 13;
            }
            curFile.drawUnfilledRect(xf, yf, xl, yl, color_set, borderPixelCount);
        }
        return 0;
    }
    return 11;
}

int32_t cmdOpr::analyseExit(uint32_t wordCount){
    if(wordCount != 1){
        return 10;
    }
    curFile.saveBMP();
    exit(0);
}

int32_t cmdOpr::analyseHelp(uint32_t wordCount, const std::vector<std::string>& cmds, bool isScriptFileMode){
    if(isScriptFileMode){
        return 0;
    }
    if(wordCount != 1 && wordCount != 2){
        return 10;
    }
    if(wordCount == 1){
        for(const std::string& cmd : commands){
            std::cout << cmd << '\t';
        }
        std::cout << std::endl;
    }else{
        const std::string& cmd = cmds[1];
        if(helpCommand.find(cmd) == helpCommand.end()){
            return 11;
        }
        for(const std::string& usage : helpCommand[cmd]){
            std::cout << usage << std::endl;
        }
    }
    return 0;
}

int32_t cmdOpr::analyseEcho(uint32_t wordCount, const std::vector<std::string>& cmds){
    if(wordCount != 2){
        return 10;
    }
    std::cout << cmds[1] << std::endl;
    return 0;
}

void cmdOpr::outPutError(int32_t errCode){
    if(errCode == 0){ return; }
    if(errCode == 10){
        std::cerr << "Wrong Format!" << std::endl;
    }else if(errCode == 11){
        std::cerr << "Unknown argument!" << std::endl;
    }else if(errCode == 12){
        std::cerr << "Pixel coordinates out of bounds! (Or number is not a number)" << std::endl;
    }else if(errCode == 13){
        std::cerr << "Number is not a number!" << std::endl;
    }else{
        std::cerr << "Unknown Error!" << std::endl;
    }
}

void cmdOpr::loopTime(bool isScriptFileMode){
    if(!isScriptFileMode){
        if(curFile.getCurFileName().empty()){
            std::cout << "(No File)";
        }
        std::cout << curFile.getCurFileName() << '>';
    }

    // read commands
    std::string cmd, tmp;
    std::vector<std::string> cmds;
    getline(std::cin, cmd);
    if(cmd.empty() && isScriptFileMode){
        exit(0);
    }
    cmd += ' '; // add a space
    uint32_t len = cmd.length(), wordCount = 0;
    for(uint32_t i = 0; i < len; i++){
        if(cmd[i] == ' '){
            cmds.push_back(tmp);
            tmp = "";
        }else{
            tmp += cmd[i];
        }
    }
    wordCount = cmds.size();

    // analyse commands
    int32_t res = 0;
    if(cmds[0] == "gen"){
        res = analyseGen(wordCount, cmds);
    }else if(cmds[0] == "open"){
        res = analyseOpen(wordCount, cmds);
    }else if(cmds[0] == "save"){
        res = analyseSave(wordCount);
    }else if(cmds[0] == "set"){
        res = analyseSet(wordCount, cmds);
    }else if(cmds[0] == "get"){
        res = analyseGet(wordCount, cmds, isScriptFileMode);
    }else if(cmds[0] == "draw"){
        res = analyseDraw(wordCount, cmds);
    }else if(cmds[0] == "exit"){
        res = analyseExit(wordCount);
    }else if(cmds[0] == "help"){
        res = analyseHelp(wordCount, cmds, isScriptFileMode);
    }else if(cmds[0] == "echo"){
        res = analyseEcho(wordCount, cmds);
    }else{
        if(!isScriptFileMode){
            std::cerr << "Unknown Command!" << std::endl;
        }
    }
    if(!isScriptFileMode){
        outPutError(res);
    }
}
