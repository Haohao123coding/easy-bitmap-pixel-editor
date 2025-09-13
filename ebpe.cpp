#include<iostream>
#include<fstream>
#include<vector>
#include<cstdint>
#include<ctime>
#include<cstdlib>
#include<string>
using namespace std;

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
	int32_t biWidth = 0;
	int32_t biHeight = 0;
	uint16_t biPlanes = 1;
	uint16_t biBitCount = 24;
	uint32_t biCompression = 0;
	uint32_t biSizeImage = 0;
	int32_t biXPelsPerMeter = 0;
	int32_t biYPelsPerMeter = 0;
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
	void openBMP(string fileName){
		// open file
		ifstream file(fileName, ios::binary);
		if(!file.is_open()){
			cerr << "No file!" << endl;
			return;
		}
		
		// read headers
	    file.read(reinterpret_cast<char*>(&curFile.fileh), sizeof(fileHeader));
    	file.read(reinterpret_cast<char*>(&curFile.infoh), sizeof(infoHeader));
    	
    	
    	// check
    	if(curFile.fileh.bfType != 0x4D42){
			cerr << "Not BMP file!" << endl;
		}
		if(
			curFile.fileh.bfReserved1 != 0 ||
			curFile.fileh.bfReserved2 != 0 ||
			curFile.fileh.bfOffBits != 54
		){ cerr << "No Support For This Format!"; return; }
		if(
			curFile.infoh.biSize != 40 ||
			curFile.infoh.biWidth < 0 ||
			curFile.infoh.biHeight < 0 ||
			curFile.infoh.biPlanes != 1 ||
			(
				curFile.infoh.biBitCount != 24 &&
				curFile.infoh.biBitCount != 32
			) ||
			curFile.infoh.biCompression != 0 ||
			curFile.infoh.biXPelsPerMeter != 0 ||
			curFile.infoh.biYPelsPerMeter != 0 ||
			curFile.infoh.biClrUsed != 0 ||
			curFile.infoh.biClrImportant != 0
		){ cerr << "No Support For This Format!"; return; }
		
		// calc
    	int width = curFile.infoh.biWidth;
    	int height = curFile.infoh.biHeight;
    	int colorByte = curFile.infoh.biBitCount / 8;
    	int rowSize = (width * colorByte + 3) / 4 * 4;
    	int paddingSize = rowSize - width * colorByte;
    	
    	// setup vectors
    	curFile.bmap.resize(height);
    	for(int i = 0; i < height; i++){
			curFile.bmap[i].resize(width);
		}
    	
    	// read pixels
    	for(int i = height - 1; i >= 0; i--){
	        for(int j = 0; j < width; j++){
	            file.read(reinterpret_cast<char*>(&curFile.bmap[i][j].b), 1);
	            file.read(reinterpret_cast<char*>(&curFile.bmap[i][j].g), 1);
	            file.read(reinterpret_cast<char*>(&curFile.bmap[i][j].r), 1);
	            if(colorByte == 4){
	                file.read(reinterpret_cast<char*>(&curFile.bmap[i][j].a), 1);
	                curFile.bmap[i][j].bit = 32;
	            }else{
	                curFile.bmap[i][j].bit = 24;
	            }
	        }
	        file.seekg(paddingSize, ios::cur);
	    }
	    
	    // misc
	    curFileName = fileName;
	}
	void calcSetBMP(){
		// calc
    	int width = curFile.infoh.biWidth;
    	int height = curFile.infoh.biHeight;
    	int colorByte = curFile.infoh.biBitCount / 8;
    	int rowSize = (width * colorByte + 3) / 4 * 4;
    	
    	// set headers
    	curFile.infoh.biWidth = width;
		curFile.infoh.biHeight = height;
		curFile.infoh.biSizeImage = rowSize * height;
		curFile.fileh.bfSize =
		sizeof(curFile.fileh) + sizeof(curFile.infoh) + curFile.infoh.biSizeImage;
	}
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
    	
    	// calc and set
    	calcSetBMP();
		
		// write headers
		file.write(reinterpret_cast<const char*>(&curFile.fileh), sizeof(fileHeader));
    	file.write(reinterpret_cast<const char*>(&curFile.infoh), sizeof(infoHeader));
    	
    	// setup vectors
    	curFile.bmap.resize(height);
    	for(int i = 0; i < height; i++){
			curFile.bmap[i].resize(width);
		}
    	
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

namespace cmdOpr{
	int hex_to_dec(char hex){
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
	int hex_2_to_dec(string hex){
		return hex_to_dec(hex[0]) * 16 + hex_to_dec(hex[1]);
	}
	color analyseColor(string str){
		int colorByte;
		if(curFile.infoh.biBitCount == 24){
			colorByte = 3;
		}else{
			colorByte = 4;
		}
		int len = str.length();
		if(str[0] != '#' || (len != 7 && len != 9)){
			// wrong format
			return colorByte == 3 ? color(0, 0, 0) : color(0, 0, 0, 0);
		}
		for(int i = 1; i < len; i++){
			if(!((str[i] >= '0' && str[i] <= '9') ||
			(str[i] >= 'a' && str[i] <= 'f'))){
				// wrong format
				return colorByte == 3 ? color(0, 0, 0) : color(0, 0, 0, 0);
			}
		}
		int r = hex_2_to_dec(str.substr(1, 2));
        int g = hex_2_to_dec(str.substr(3, 2));
        int b = hex_2_to_dec(str.substr(5, 2));
        int a;
        if(colorByte == 4) a = hex_2_to_dec(str.substr(7, 2));
        if(colorByte == 3){
			return color(r, g, b);
		}else{
			return color(r, g, b, a);
		}
	}
	void loopTime(){
		if(curFileName == ""){
			cout << "(No File)";
		}
		cout << curFileName << '>';
		
		// read commands
		string cmd, tmp = "";
		vector<string> cmds;
		getline(cin, cmd);
		cmd += ' '; // add a space
		int len = cmd.length(), wordCount = 0;
		for(int i = 0; i < len; i++){
			if(cmd[i] == ' '){
				cmds.push_back(tmp);
				tmp = "";
			}else{
				tmp += cmd[i];
			}
		}
		wordCount = cmds.size();
		
		// analyse commands
		if(cmds[0] == "gen"){
			if(wordCount != 4 && wordCount != 5){
				cerr << "Wrong Format!" << endl;
			}else{
				string fileName = cmds[1];
				int width = stoi(cmds[2]);
				int height = stoi(cmds[3]);
				color filling;
				if(wordCount == 5){
					filling = analyseColor(cmds[4]);
				}else{
					filling = color(255, 255, 255, 255);
				}
				bmpOpr::genBMP(width, height, filling, fileName);
			}
		}else if(cmds[0] == "open"){
			if(wordCount != 2){
				cerr << "Wrong Format!" << endl;
			}else{
				bmpOpr::openBMP(cmds[1]);
			}
		}else if(cmds[0] == "save"){
			if(wordCount != 1){
				cerr << "Wrong Format!" << endl;
			}else{
				bmpOpr::saveBMP();
			}
		}else if(cmds[0] == "set"){
			if(wordCount != 3){
				cerr << "Wrong Format!" << endl;
			}else{
				bool flagSuccess = true;
				if(cmds[1] == "width"){
					curFile.infoh.biWidth = stoi(cmds[2]);
				}else if(cmds[1] == "height"){
					curFile.infoh.biHeight = stoi(cmds[2]);
				}else{
					cerr << "Unknown argument!" << endl;
					flagSuccess = false;
				}
				if(flagSuccess) bmpOpr::calcSetBMP();
			}
		}else if(cmds[0] == "get"){
			if(wordCount != 2){
				cerr << "Wrong Format!" << endl;
			}else{
				bool flagSuccess = true;
				int answer;
				if(cmds[1] == "width"){
					answer = curFile.infoh.biWidth;
				}else if(cmds[1] == "height"){
					answer = curFile.infoh.biHeight;
				}else{
					cerr << "Unknown argument!" << endl;
					flagSuccess = false;
				}
				if(flagSuccess) cout << cmds[1] << ": " << answer << endl;
			}
		}else if(cmds[0] == "draw"){
			if(wordCount < 2){
				cerr << "Wrong Format!" << endl;
			}
			if(cmds[1] == "pixel"){
				if(wordCount != 5){
					cerr << "Wrong Format!" << endl;
				}else{
					int x = stoi(cmds[2]);
					int y = stoi(cmds[3]);
					color color_set = analyseColor(cmds[4]);
					bmpOpr::editPixel(x, y, color_set);
				}
			}else if(cmds[1] == "rect"){
				if(wordCount != 7){
					cerr << "Wrong Format!" << endl;
				}else{
					int xf = stoi(cmds[2]);
					int yf = stoi(cmds[3]);
					int xl = stoi(cmds[4]);
					int yl = stoi(cmds[5]);
					color color_set = analyseColor(cmds[6]);
					bmpOpr::drawRect(xf, yf, xl, yl, color_set);
				}
			}
		}else if(cmds[0] == "exit"){
			if(wordCount != 1){
				cerr << "Wrong Format!" << endl;
			}else{
				bmpOpr::saveBMP();
				exit(0);
			}
		}else{
			cerr << "Unknown Command!" << endl;
		}
	}
}

int main(){
	while(true){
		cmdOpr::loopTime();
	}
	return 0;
}
