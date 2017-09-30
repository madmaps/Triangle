#include "BitMapHeader.h"

bitMapHeader::bitMapHeader(char* inputBuffer,int* sPosition,int* ePosition)
{
	char tempChar[5];

	for(int i=0;i<=1;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	id=reinterpret_cast<short&>(tempChar);

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	length=reinterpret_cast<long&>(tempChar);

	*sPosition+=4;

	for(int i=0;i<=3;i++)
	{
		tempChar[i]=inputBuffer[(*sPosition)++];
	}
	offset=reinterpret_cast<long&>(tempChar);
	*ePosition=*sPosition;
}
