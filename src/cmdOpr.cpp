#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>
#include <string>

#include "bmpFile.h"
#include "cmdOpr.h"

int32_t cmdOpr::hex_to_dec(char hex){
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
int32_t cmdOpr::hex_2_to_dec(string hex){
    return hex_to_dec(hex[0]) * 16 + hex_to_dec(hex[1]);
}
color cmdOpr::analyseColor(string str){
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
    uint8_t r = static_cast<uint8_t>(hex_2_to_dec(str.substr(1, 2)));
    uint8_t g = static_cast<uint8_t>(hex_2_to_dec(str.substr(3, 2)));
    uint8_t b = static_cast<uint8_t>(hex_2_to_dec(str.substr(5, 2)));
    uint8_t a = 0;
    if(colorByte == 4) a = static_cast<uint8_t>(hex_2_to_dec(str.substr(7, 2)));
    if(colorByte == 3){
        return {r, g, b};
    }else{
        return {r, g, b, a};
    }
}

int32_t cmdOpr::analyseGen(uint32_t wordCount, vector<string> cmds){
    if(wordCount != 4 && wordCount != 5){
        return 10;
    }
    const string& fileName = cmds[1];
    int32_t width = stoi(cmds[2]);
    int32_t height = stoi(cmds[3]);
    color filling;
    if(wordCount == 5){
        filling = analyseColor(cmds[4]);
    }else{
        filling = color(255, 255, 255, 255);
    }
    curFile = bmpFile(width, height, filling, fileName);
    return 0;
}
int32_t cmdOpr::analyseOpen(uint32_t wordCount, const vector<string>& cmds){
    if(wordCount != 2){
        return 10;
    }
    curFile.openBMP(cmds[1]);
    return 0;
}
int32_t cmdOpr::analyseSave(uint32_t wordCount, const vector<string>& cmds){
    if(wordCount != 1){
        return 10;
    }
    curFile.saveBMP();
    return 0;
}
int32_t cmdOpr::analyseSet(uint32_t wordCount, const vector<string>& cmds){
    if(wordCount != 3){
        return 10;
    }
    infoHeader ih = curFile.getInfoHeader();
    if(cmds[1] == "width"){
        ih.biWidth = stoi(cmds[2]);
    }else if(cmds[1] == "height"){
        ih.biHeight = stoi(cmds[2]);
    }else{
        return 11;
    }
    curFile.setInfoHeader(ih);
    curFile.calcSetBMP();
    return 0;
}
int32_t cmdOpr::analyseGet(uint32_t wordCount, const vector<string>& cmds){
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
    cout << cmds[1] << ": " << answer << endl;
    return 0;
}
int32_t cmdOpr::analyseDraw(uint32_t wordCount, const vector<string>& cmds){
    if(wordCount < 2){
        return 10;
    }
    if(cmds[1] == "pixel"){
        if(wordCount != 5){
            return 10;
        }
        int32_t x = stoi(cmds[2]);
        int32_t y = stoi(cmds[3]);
        color color_set = analyseColor(cmds[4]);
        if(x < 0 || x >= curFile.getInfoHeader().biHeight ||
        y < 0 || y >= curFile.getInfoHeader().biWidth){
            return 12;
        }
        curFile.editPixel(x, y, color_set);
        return 0;
    }else if(cmds[1] == "rect"){
        if(wordCount != 7 && wordCount != 8 && wordCount != 9){
            return 10;
        }
        int32_t xf, yf, xl, yl, ptr;
        color color_set;
        if(cmds[2] != "filled" && cmds[2] != "unfilled"){
            ptr = 2;
        }else{
            ptr = 3;
        }
        xf = stoi(cmds[ptr++]);
        yf = stoi(cmds[ptr++]);
        xl = stoi(cmds[ptr++]);
        yl = stoi(cmds[ptr++]);
        color_set = analyseColor(cmds[ptr++]);
        if(xf < 0 || xf >= curFile.getInfoHeader().biHeight ||
        xl < 0 || xl >= curFile.getInfoHeader().biHeight ||
        yf < 0 || yf >= curFile.getInfoHeader().biWidth ||
        yl < 0 || yl >= curFile.getInfoHeader().biWidth){
            return 12;
        }
        if(wordCount == 7 || (wordCount == 8 && cmds[2] == "filled")){
            curFile.drawRect(xf, yf, xl, yl, color_set);
        }else{
            int32_t borderPixelCount;
            if(wordCount != 9){
                borderPixelCount = 1;
            }else{
                borderPixelCount = stoi(cmds[8]);
            }
            curFile.drawUnfilledRect(xf, yf, xl, yl, color_set, borderPixelCount);
        }
        return 0;
    }
    return 11;
}
int32_t cmdOpr::analyseExit(uint32_t wordCount, const vector<string>& cmds){
    if(wordCount != 1){
        return 10;
    }
    curFile.saveBMP();
    exit(0);
}

void cmdOpr::outPutError(int32_t errCode){
    if(errCode == 10){
        cerr << "Wrong Format!" << endl;
    }else if(errCode == 11){
        cerr << "Unknown argument!" << endl;
    }else if(errCode == 12){
        cerr << "Pixel coordinates out of bounds!" << endl;
    }else{
        cerr << "Unknown Error!" << endl;
    }
}

void cmdOpr::loopTime(){
    if(curFileName.empty()){
        cout << "(No File)";
    }
    cout << curFileName << '>';

    // read commands
    string cmd, tmp;
    vector<string> cmds;
    getline(cin, cmd);
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
        res = analyseSave(wordCount, cmds);
    }else if(cmds[0] == "set"){
        res = analyseSet(wordCount, cmds);
    }else if(cmds[0] == "get"){
        res = analyseGet(wordCount, cmds);
    }else if(cmds[0] == "draw"){
        res = analyseDraw(wordCount, cmds);
    }else if(cmds[0] == "exit"){
        res = analyseExit(wordCount, cmds);
    }else{
        cerr << "Unknown Command!" << endl;
    }
    outPutError(res);
}
