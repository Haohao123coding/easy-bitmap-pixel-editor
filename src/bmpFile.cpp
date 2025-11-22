// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#include <iostream>
#include <fstream>
#include <utility>

#include "bmpFile.h"

color::color(){
    bit = 24;
    r = 0; g = 0; b = 0; a = 0;
}
color::color(uint8_t x, uint8_t y, uint8_t z){
    bit = 24; a = 0;
    r = x; g = y; b = z;
}
color::color(uint8_t x, uint8_t y, uint8_t z, uint8_t t){
    bit = 32;
    r = x; g = y; b = z; a = t;
}
bool color::operator< (const color& rhs) const {
    if(r != rhs.r) return r < rhs.r;
    if(g != rhs.g) return g < rhs.g;
    if(b != rhs.b) return b < rhs.b;
    if(bit == 32 && a != rhs.a) return a < rhs.a;
    return false;
}

bigColor::bigColor() = default;
bigColor::bigColor(color c){
    dict[c] = 1;
}

infoHeader::infoHeader() = default;
infoHeader::infoHeader(int32_t width, int32_t height, bool hasAlpha){
    biWidth = width;
    biHeight = height;
    if(hasAlpha) biBitCount = 32;
    else biBitCount = 24;
}

bmpFile::bmpFile() = default;
bmpFile::bmpFile(int32_t bWidth, int32_t bHeight, bool bHasAlpha){
    infoh = infoHeader(bWidth, bHeight, bHasAlpha);

    // setup vectors
    bmap.resize(bHeight);
    for(int32_t i = 0; i < bHeight; i++){
        bmap[i].resize(bWidth);
    }
}
bmpFile::bmpFile(int32_t width, int32_t height, color filling, const std::string& fileName){
    // calc
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
    sizeof(fh) + sizeof(ih) + ih.biSizeImage;
    fileh = fh;
    infoh = ih;

    // setup pixels
    std::vector<std::vector<color>> bm;
    std::vector<color> tmp;
    tmp.reserve(width);
    for(int32_t i = 0; i < width; i++){
        tmp.push_back(filling);
    }
    bm.reserve(height);
    for(int32_t i = 0; i < height; i++){
        bm.push_back(tmp);
    }
    bmap = bm;

    // save file
    curFileName = fileName;
    saveBMP(true);
}


fileHeader bmpFile::getFileHeader(){ return fileh; }
infoHeader bmpFile::getInfoHeader(){ return infoh; }
std::vector<std::vector<color>> bmpFile::getBmap(){ return bmap; }
std::string bmpFile::getCurFileName(){ return curFileName; }

void bmpFile::setFileHeader(const fileHeader& data){ fileh = data; }
void bmpFile::setInfoHeader(const infoHeader& data){ infoh = data; }
void bmpFile::setBmap(std::vector<std::vector<color>> data){ bmap = std::move(data); }
void bmpFile::setCurFileName(const std::string& fileName){ curFileName = fileName; }

void bmpFile::editPixel(int32_t x, int32_t y, color c){
    // x, y is from zero
    std::vector<std::vector<color>> bm = getBmap();
    bm[x][y] = c;
    setBmap(bm);
}
void bmpFile::drawRect(int32_t xf, int32_t yf, int32_t xl, int32_t yl, color c){
    // X first; X last; Y first; Y last
    // x, y is from zero
    for(int32_t i = xf; i <= xl; i++){
        for(int32_t j = yf; j <= yl; j++){
            editPixel(i, j, c);
        }
    }
}
void bmpFile::drawUnfilledRect(int32_t xf, int32_t yf, int32_t xl, int32_t yl, color c, int32_t borderPixelCount){
    // X first; X last; Y first; Y last
    // x, y is from zero
    drawRect(xf, yf, xf + borderPixelCount - 1, yl, c);
    drawRect(xf, yf, xl, yf + borderPixelCount - 1, c);
    drawRect(xl - borderPixelCount + 1, yf, xl, yl, c);
    drawRect(xf, yl - borderPixelCount + 1, xl, yl, c);
}
void bmpFile::openBMP(const std::string& fileName, bool isScriptFileMode){
    // open file
    std::ifstream file(fileName, std::ios::binary);
    if(!file.is_open() && !isScriptFileMode){
        std::cerr << "No file!" << std::endl;
        return;
    }

    // read headers
    file.read(reinterpret_cast<char*>(&fileh), sizeof(fileHeader));
    file.read(reinterpret_cast<char*>(&infoh), sizeof(infoHeader));

    // check
    if(fileh.bfType != 0x4D42 && !isScriptFileMode){
        std::cerr << "Not BMP file!" << std::endl;
        return;
    }
    if((
        fileh.bfReserved1 != 0 ||
        fileh.bfReserved2 != 0 ||
        fileh.bfOffBits != 54
    ) && !isScriptFileMode){ std::cerr << "No Support For This Format!"; return; }
    if((
        infoh.biSize != 40 ||
        infoh.biWidth < 0 ||
        infoh.biHeight < 0 ||
        infoh.biPlanes != 1 ||
        (
            infoh.biBitCount != 24 &&
            infoh.biBitCount != 32
        ) ||
        infoh.biCompression != 0 ||
        infoh.biXPixelsPerMeter != 0 ||
        infoh.biYPixelsPerMeter != 0 ||
        infoh.biClrUsed != 0 ||
        infoh.biClrImportant != 0
    ) && !isScriptFileMode){ std::cerr << "No Support For This Format!"; return; }

    // calc
    int32_t width = infoh.biWidth;
    int32_t height = infoh.biHeight;
    uint16_t colorByte = infoh.biBitCount / 8;
    int32_t rowSize = (width * colorByte + 3) / 4 * 4;
    int32_t paddingSize = rowSize - width * colorByte;

    // setup std::vectors
    std::vector<std::vector<color>> bm;
    bm.resize(height);
    for(int32_t i = 0; i < height; i++){
        bm[i].resize(width);
    }
    bmap = bm;

    // read pixels
    for(int32_t i = height - 1; i >= 0; i--){
        for(int32_t j = 0; j < width; j++){
            file.read(reinterpret_cast<char*>(&bmap[i][j].b), 1);
            file.read(reinterpret_cast<char*>(&bmap[i][j].g), 1);
            file.read(reinterpret_cast<char*>(&bmap[i][j].r), 1);
            if(colorByte == 4){
                file.read(reinterpret_cast<char*>(&bmap[i][j].a), 1);
                bmap[i][j].bit = 32;
            }else{
                bmap[i][j].bit = 24;
            }
        }
        file.seekg(paddingSize, std::ios::cur);
    }

    // misc
    curFileName = fileName;
}

void bmpFile::calcSetBMP(){
    // calc
    int32_t width = infoh.biWidth;
    int32_t height = infoh.biHeight;
    uint16_t colorByte = infoh.biBitCount / 8;
    int32_t rowSize = (width * colorByte + 3) / 4 * 4;

    // set headers
    infoh.biWidth = width;
    infoh.biHeight = height;
    infoh.biSizeImage = rowSize * height;
    fileh.bfSize = sizeof(fileh) + sizeof(infoh) + infoh.biSizeImage;
}

void bmpFile::saveBMP(bool isScriptFileMode){
    // open file
    std::ofstream file(curFileName, std::ios::binary);
    if(!file.is_open() && !isScriptFileMode){
        std::cerr << "No file!" << std::endl;
        return;
    }

    // calc
    int32_t width = infoh.biWidth;
    int32_t height = infoh.biHeight;
    uint16_t colorByte = infoh.biBitCount / 8;
    int32_t rowSize = (width * colorByte + 3) / 4 * 4;
    int32_t paddingSize = rowSize - width * colorByte;

    // calc and set
    calcSetBMP();

    // write headers
    fileHeader fh = fileh;
    infoHeader ih = infoh;
    file.write(reinterpret_cast<char*>(&fh), sizeof(fileHeader));
    file.write(reinterpret_cast<char*>(&ih), sizeof(infoHeader));

    // setup std::vectors
    bmap.resize(height);
    for(int32_t i = 0; i < height; i++){
        bmap[i].resize(width);
    }

    // write pixels
    for(int32_t i = height - 1; i >= 0; i--){
        for(int32_t j = 0; j < width; j++){
            file.write(reinterpret_cast<const char*>(&bmap[i][j].b), 1);
            file.write(reinterpret_cast<const char*>(&bmap[i][j].g), 1);
            file.write(reinterpret_cast<const char*>(&bmap[i][j].r), 1);
            if(colorByte == 4){
                file.write(reinterpret_cast<const char*>(&bmap[i][j].a), 1);
            }
        }
        for(int32_t j = 0; j < paddingSize; j++){
            uint8_t padding = 0;
            file.write(reinterpret_cast<const char*>(&padding), 1);
        }
    }
}
