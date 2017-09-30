#ifndef __BMPLOADER__
#define __BMPLOADER__

#include <fstream>
#include "BitMapHeader.h"
#include "DibHeader.h"

class bmpLoader
{
public:
	bmpLoader();
	~bmpLoader();
	bool loadFile(char* info);
	unsigned char* getData();
	int getSizeOfData();
	int getWidth();
	int getHeigth();
	int getColorBits();
	void reverseRGB();
private:
	std::ifstream file;
	bitMapHeader* t_bitMapHeader;
	dibHeader* t_dibHeader;
	unsigned char* t_data;
	char* buffer;
	int fileSize;
	int* startPosition;
	int* endPosition;
	int padding;
	int imageSize;
	int numOfColorBits;
};

#endif
