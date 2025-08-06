#include<iostream>
#include<fstream>
#include<vector>
#include<cstdint>
#include<ctime>
#include<cstdlib>
#include<string>
using namespace std;

namespace rnd{
	void setupSeed(){
		srand(time(0));
	}
	int randInt(int a, int b){
		return rand() % (b - a + 1) + a;
	}
};

struct color{
	int bit; // 24 or 32
	int r, g, b, a;
	color(){}
	color(int x, int y, int z){
		bit = 24;
		r = x; g = y; b = z;
	}
	color(int x, int y, int z, int t){
		bit = 32;
		r = x; g = y; b = z; a = t;
	}
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
	int32_t	biWidth = 0;
	int32_t	biHeight = 0;
	uint16_t biPlanes = 1;
	uint16_t biBitCount = 24;
	uint32_t biCompression = 0;
	uint32_t biSizeImage = 0;
	int32_t	biXPelsPerMeter = 0;
	int32_t	biYPelsPerMeter = 0;
	uint32_t biClrUsed = 0;
	uint32_t biClrImportant = 0;
	infoHeader(){}
	infoHeader(int32_t width, int32_t height, bool hasAlpha){
		biWidth = width;
		biHeight = height;
		if(hasAlpha) biBitCount = 32;
		else biBitCount = 24;
	}
};

struct bmpFile{
	fileHeader fileh;
	infoHeader infoh;
	vector<vector<color>> bmap;
	bmpFile(){}
	bmpFile(int32_t bWidth, int32_t bHeight, bool bHasAlpha){
		infoh = infoHeader(bWidth, bHeight, bHasAlpha);
	}
};

#pragma pack(pop) // Restore byte alignment

bmpFile curFile;
string curFileName;

namespace bmpOpr{
	void saveBMP(){
		// open file
		ofstream file(curFileName, ios::binary);
		if(!file.is_open()){
			cerr << "No file!" << endl;
			return;
		}
    	
    	// calc
    	int width = curFile.infoh.biWidth;
    	int height = curFile.infoh.biHeight;
    	int colorByte = curFile.infoh.biBitCount / 8;
    	int rowSize = (width * colorByte + 3) / 4 * 4;
    	int paddingSize = rowSize - width * colorByte;
    	
    	// set headers
    	curFile.infoh.biWidth = width;
		curFile.infoh.biHeight = height;
		curFile.infoh.biSizeImage = rowSize * height;
		curFile.fileh.bfSize =
		sizeof(curFile.fileh) + sizeof(curFile.infoh) + curFile.infoh.biSizeImage;
		
		// write headers
		file.write(reinterpret_cast<const char*>(&curFile.fileh), sizeof(fileHeader));
    	file.write(reinterpret_cast<const char*>(&curFile.infoh), sizeof(infoHeader));
    	
    	// write pixels
    	for(int i = height - 1; i >= 0; i--){
			for(int j = 0; j < width; j++){
	            file.write(reinterpret_cast<const char*>(&curFile.bmap[i][j].b), 1);
	            file.write(reinterpret_cast<const char*>(&curFile.bmap[i][j].g), 1);
	            file.write(reinterpret_cast<const char*>(&curFile.bmap[i][j].r), 1);
			}
			for(int j = 0; j < paddingSize; j++){
	            uint8_t padding = 0;
	            file.write(reinterpret_cast<const char*>(&padding), 1);
	        }
		}
	}
	void genBMP(int32_t width, int32_t height, color filling, string fileName){
		// calc
		bmpFile newFile;
		int colorByte = filling.bit / 8;
		int rowSize = (width * colorByte + 3) / 4 * 4;
		
		// setup file header and info header
		newFile.infoh.biWidth = width;
		newFile.infoh.biHeight = height;
		newFile.infoh.biBitCount = filling.bit;
		newFile.infoh.biSizeImage = rowSize * height;
		newFile.fileh.bfSize =
		sizeof(newFile.fileh) + sizeof(newFile.infoh) + newFile.infoh.biSizeImage;
		
		// setup pixels
		vector<color> tmp;
		for(int i = 0; i < width; i++){
			tmp.push_back(filling);
		}
		for(int i = 0; i < height; i++){
			newFile.bmap.push_back(tmp);
		}
		
		// save file
		curFile = newFile;
		curFileName = fileName;
		saveBMP();
	}
	void editPixel(int32_t x, int32_t y, color c){
		// x, y is from zero
		curFile.bmap[x][y] = c;
	}
	void drawRect(int32_t xf, int32_t yf, int32_t xl, int32_t yl, color c){
		// X first; X last; Y first; Y last
		// x, y is from zero
		for(int i = xf; i <= xl; i++){
			for(int j = yf; j <= yl; j++){
				editPixel(i, j, c);
			}
		}
	}
}

int main(){
	bmpOpr::genBMP(10, 10, color(0, 128, 255), "haha.bmp");
	bmpOpr::editPixel(2, 2, color(255, 64, 0));
	bmpOpr::drawRect(5, 5, 8, 7, color(64, 255, 32));
	bmpOpr::saveBMP();
    return 0;
}
