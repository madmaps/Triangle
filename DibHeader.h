#ifndef __DIBHEADER__
#define __DIBHEADER__

class dibHeader
{
public:
	dibHeader(char* inputBuffer,int* sPosition,int* ePosition);
	unsigned long headerSize;
	int width;
	int height;
	unsigned short numColorPlanes;
	unsigned short bitsPerPix;
	long compression;
	unsigned long imageSize;
	int xPixPerMeter;
	int yPixPerMeter;
	long numColorPal;
	long numImpCols;
};

#endif

