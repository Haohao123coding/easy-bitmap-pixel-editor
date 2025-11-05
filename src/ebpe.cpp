#include <iostream>
#include <fstream>
#include <utility>
#include <vector>
#include <cstdint>
#include <string>

#include "bmpFile.h"

using namespace std;

bmpFile curFile;
string curFileName;

namespace bmpOpr{
    void openBMP(const string& fileName){
        // open file
        ifstream file(fileName, ios::binary);
        if(!file.is_open()){
            cerr << "No file!" << endl;
            return;
        }

        // read headers
        fileHeader fh = curFile.getFileHeader();
        infoHeader ih = curFile.getInfoHeader();
        file.read(reinterpret_cast<char*>(&fh), sizeof(fileHeader));
        file.read(reinterpret_cast<char*>(&ih), sizeof(infoHeader));

        // check
        if(curFile.getFileHeader().bfType != 0x4D42){
            cerr << "Not BMP file!" << endl;
        }
        if(
            curFile.getFileHeader().bfReserved1 != 0 ||
            curFile.getFileHeader().bfReserved2 != 0 ||
            curFile.getFileHeader().bfOffBits != 54
        ){ cerr << "No Support For This Format!"; return; }
        if(
            curFile.getInfoHeader().biSize != 40 ||
            curFile.getInfoHeader().biWidth < 0 ||
            curFile.getInfoHeader().biHeight < 0 ||
            curFile.getInfoHeader().biPlanes != 1 ||
            (
                curFile.getInfoHeader().biBitCount != 24 &&
                curFile.getInfoHeader().biBitCount != 32
            ) ||
            curFile.getInfoHeader().biCompression != 0 ||
            curFile.getInfoHeader().biXPixelsPerMeter != 0 ||
            curFile.getInfoHeader().biYPixelsPerMeter != 0 ||
            curFile.getInfoHeader().biClrUsed != 0 ||
            curFile.getInfoHeader().biClrImportant != 0
        ){ cerr << "No Support For This Format!"; return; }

        // calc
        int32_t width = curFile.getInfoHeader().biWidth;
        int32_t height = curFile.getInfoHeader().biHeight;
        uint16_t colorByte = ih.biBitCount / 8;
        int32_t rowSize = (width * colorByte + 3) / 4 * 4;
        int32_t paddingSize = rowSize - width * colorByte;

        // setup vectors
        vector<vector<color>> bm;
        bm.resize(height);
        for(int32_t i = 0; i < height; i++){
            bm[i].resize(width);
        }
        curFile.setBmap(bm);

        // read pixels
        for(int32_t i = height - 1; i >= 0; i--){
            for(int32_t j = 0; j < width; j++){
                file.read(reinterpret_cast<char*>(&curFile.getBmap()[i][j].b), 1);
                file.read(reinterpret_cast<char*>(&curFile.getBmap()[i][j].g), 1);
                file.read(reinterpret_cast<char*>(&curFile.getBmap()[i][j].r), 1);
                if(colorByte == 4){
                    file.read(reinterpret_cast<char*>(&curFile.getBmap()[i][j].a), 1);
                    curFile.getBmap()[i][j].bit = 32;
                }else{
                    curFile.getBmap()[i][j].bit = 24;
                }
            }
            file.seekg(paddingSize, ios::cur);
        }

        // misc
        curFileName = fileName;
    }
    void calcSetBMP(){
        // calc
        int32_t width = curFile.getInfoHeader().biWidth;
        int32_t height = curFile.getInfoHeader().biHeight;
        uint16_t colorByte = curFile.getInfoHeader().biBitCount / 8;
        int32_t rowSize = (width * colorByte + 3) / 4 * 4;

        // set headers
        fileHeader fh = curFile.getFileHeader();
        infoHeader ih = curFile.getInfoHeader();
        ih.biWidth = width;
        ih.biHeight = height;
        ih.biSizeImage = rowSize * height;
        fh.bfSize =
        sizeof(curFile.getFileHeader()) + sizeof(curFile.getInfoHeader()) + curFile.getInfoHeader().biSizeImage;
        curFile.setFileHeader(fh);
        curFile.setInfoHeader(ih);
    }
    void saveBMP(){
        // open file
        ofstream file(curFileName, ios::binary);
        if(!file.is_open()){
            cerr << "No file!" << endl;
            return;
        }

        // calc
        int32_t width = curFile.getInfoHeader().biWidth;
        int32_t height = curFile.getInfoHeader().biHeight;
        uint16_t colorByte = curFile.getInfoHeader().biBitCount / 8;
        int32_t rowSize = (width * colorByte + 3) / 4 * 4;
        int32_t paddingSize = rowSize - width * colorByte;

        // calc and set
        calcSetBMP();

        // write headers
        fileHeader fh = curFile.getFileHeader();
        infoHeader ih = curFile.getInfoHeader();
        file.write(reinterpret_cast<char*>(&fh), sizeof(fileHeader));
        file.write(reinterpret_cast<char*>(&ih), sizeof(infoHeader));

        // setup vectors
        vector<vector<color>> bm;
        bm.resize(height);
        for(int32_t i = 0; i < height; i++){
            bm[i].resize(width);
        }
        curFile.setBmap(bm);

        // write pixels
        for(int32_t i = height - 1; i >= 0; i--){
            for(int32_t j = 0; j < width; j++){
                file.write(reinterpret_cast<const char*>(&curFile.getBmap()[i][j].b), 1);
                file.write(reinterpret_cast<const char*>(&curFile.getBmap()[i][j].g), 1);
                file.write(reinterpret_cast<const char*>(&curFile.getBmap()[i][j].r), 1);
                if(colorByte == 4){
                    file.write(reinterpret_cast<const char*>(&curFile.getBmap()[i][j].a), 1);
                }
            }
            for(int32_t j = 0; j < paddingSize; j++){
                uint8_t padding = 0;
                file.write(reinterpret_cast<const char*>(&padding), 1);
            }
        }
    }
    void genBMP(int32_t width, int32_t height, color filling, string fileName){
        // calc
        bmpFile newFile;
        uint16_t colorByte = filling.bit / 8;
        int32_t rowSize = (width * colorByte + 3) / 4 * 4;

        // setup file header and info header
        fileHeader fh;
        infoHeader ih;
        ih.biWidth = width;
        ih.biHeight = height;
        ih.biBitCount = filling.bit;
        ih.biSizeImage = rowSize * height;
        fh.bfSize =
        sizeof(newFile.getFileHeader()) + sizeof(newFile.getInfoHeader()) + newFile.getInfoHeader().biSizeImage;
        newFile.setFileHeader(fh);
        newFile.setInfoHeader(ih);

        // setup pixels
        vector<vector<color>> bm;
        vector<color> tmp;
        tmp.reserve(width);
        for(int32_t i = 0; i < width; i++){
            tmp.push_back(filling);
        }
        bm.reserve(height);
        for(int32_t i = 0; i < height; i++){
            bm.push_back(tmp);
        }
        newFile.setBmap(bm);

        // save file
        curFile = newFile;
        curFileName = std::move(fileName);
        saveBMP();
    }
}

namespace cmdOpr{
    int32_t hex_to_dec(char hex){
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
    int32_t hex_2_to_dec(string hex){
        return hex_to_dec(hex[0]) * 16 + hex_to_dec(hex[1]);
    }
    color analyseColor(string str){
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

    int32_t analyseGen(uint32_t wordCount, vector<string> cmds){
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
        bmpOpr::genBMP(width, height, filling, fileName);
        return 0;
    }
    int32_t analyseOpen(uint32_t wordCount, const vector<string>& cmds){
        if(wordCount != 2){
            return 10;
        }
        bmpOpr::openBMP(cmds[1]);
        return 0;
    }
    int32_t analyseSave(uint32_t wordCount, const vector<string>& cmds){
        if(wordCount != 1){
            return 10;
        }
        bmpOpr::saveBMP();
        return 0;
    }
    int32_t analyseSet(uint32_t wordCount, const vector<string>& cmds){
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
        bmpOpr::calcSetBMP();
        return 0;
    }
    int32_t analyseGet(uint32_t wordCount, const vector<string>& cmds){
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
    int32_t analyseDraw(uint32_t wordCount, const vector<string>& cmds){
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
    int32_t analyseExit(uint32_t wordCount, const vector<string>& cmds){
        if(wordCount != 1){
            return 10;
        }
        bmpOpr::saveBMP();
        exit(0);
    }

    void outPutError(int32_t errCode){
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

    void loopTime(){
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
}

int main(){
    while(true){
        cmdOpr::loopTime();
    }
    return 0;
}
