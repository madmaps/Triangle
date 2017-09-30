#ifndef __BITMAPHEADER__
#define __BITMAPHEADER__

class bitMapHeader
{
public:
	bitMapHeader(char* inputBuffer,int*sPosition,int* ePosition);
	unsigned short id;
	unsigned long length;
	unsigned long offset;
};

#endif
