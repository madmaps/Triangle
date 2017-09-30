#include "DibHeader.h"

dibHeader::dibHeader(char* inputBuffer,int* sPosition,int* ePosition)
{
	int start=*sPosition;
	char tempChar[5];

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	headerSize=reinterpret_cast<long&>(tempChar);
	*ePosition=start+headerSize;

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	width=reinterpret_cast<int&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	height=reinterpret_cast<int&>(tempChar);

	for(int i=0;i<=1;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	numColorPlanes=reinterpret_cast<short&>(tempChar);

	for(int i=0;i<=1;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	bitsPerPix=reinterpret_cast<short&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	compression=reinterpret_cast<long&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	imageSize=reinterpret_cast<long&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	xPixPerMeter=reinterpret_cast<int&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	yPixPerMeter=reinterpret_cast<int&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	numColorPal=reinterpret_cast<long&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	numImpCols=reinterpret_cast<long&>(tempChar);
}
