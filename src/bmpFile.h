// *** EBPE ***
// SPDX-License-Identifier: MIT
// Copyright (c) 2025 Haohao123coding

#ifndef EASY_BITMAP_PIXEL_EDITOR_BMPFILE_H
#define EASY_BITMAP_PIXEL_EDITOR_BMPFILE_H

#include <cstdint>
#include <string>
#include <vector>
#include <map>

struct color{
    uint16_t bit; // 24 or 32
    uint8_t r, g, b, a;
    color();
    color(uint8_t x, uint8_t y, uint8_t z);
    color(uint8_t x, uint8_t y, uint8_t z, uint8_t t);

    bool operator< (const color& rhs) const;
};

struct bigColor{
    std::map<color, uint32_t> dict;
    bigColor();
    explicit bigColor(color c);
};

#pragma pack(push, 1) // Disable byte alignment

struct fileHeader{
    uint16_t bfType = 0x4D42;
    uint32_t bfSize = 0;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;
};

struct infoHeader{
    uint32_t biSize = 40;
    int32_t biWidth = 0;
    int32_t biHeight = 0;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;
    int32_t biXPixelsPerMeter = 0;
    int32_t biYPixelsPerMeter = 0;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
    infoHeader();
    infoHeader(int32_t width, int32_t height, bool hasAlpha);
};

class bmpFile{
private:
    fileHeader fileh;
    infoHeader infoh;
    std::vector<std::vector<color>> bmap;
    std::string curFileName;
public:
    bmpFile();
    bmpFile(int32_t bWidth, int32_t bHeight, bool bHasAlpha);
    bmpFile(int32_t width, int32_t height, color filling, const std::string& fileName);
    bmpFile(int32_t width, int32_t height, bigColor filling, const std::string& fileName);

    fileHeader getFileHeader();
    infoHeader getInfoHeader();
    std::vector<std::vector<color>> getBmap();
    std::string getCurFileName();
    void setFileHeader(const fileHeader& data);
    void setInfoHeader(const infoHeader& data);
    void setBmap(std::vector<std::vector<color>> data);
    void setCurFileName(const std::string& fileName);
    
    void editPixel(int32_t x, int32_t y, color c);
    void drawRect(int32_t xf, int32_t yf, int32_t xl, int32_t yl, color c);
    void drawUnfilledRect(int32_t xf, int32_t yf, int32_t xl, int32_t yl, color c, int32_t borderPixelCount);

    void openBMP(const std::string& fileName, bool isScriptFileMode);
    void calcSetBMP();
    void saveBMP(bool isScriptFileMode);
};

#pragma pack(pop) // Restore byte alignment

#endif //EASY_BITMAP_PIXEL_EDITOR_BMPFILE_H
