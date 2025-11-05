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

fileHeader bmpFile::getFileHeader(){ return fileh; }
infoHeader bmpFile::getInfoHeader(){ return infoh; }
vector<vector<color>> bmpFile::getBmap(){ return bmap; }
void bmpFile::setFileHeader(const fileHeader& data){ fileh = data; }
void bmpFile::setInfoHeader(const infoHeader& data){ infoh = data; }
void bmpFile::setBmap(vector<vector<color>> data){ bmap = std::move(data); }

void bmpFile::editPixel(int32_t x, int32_t y, color c){
    // x, y is from zero
    vector<vector<color>> bm = getBmap();
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
