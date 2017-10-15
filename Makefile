CC=g++

CFLAGS = -c -Wall -std=c++11

LIBS = -lGL -lX11 -lassimp


all: Triangle

Triangle: Triangle.o BmpLoader.o DibHeader.o BitMapHeader.o
	$(CC) DibHeader.o BitMapHeader.o BmpLoader.o Triangle.o  -o Triangle.out $(LIBS)

Triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) Triangle.cpp 
	
BmpLoader.o: BmpLoader.cpp
	$(CC) $(CFLAGS) BmpLoader.cpp
	
DibHeader.o: DibHeader.cpp
	$(CC) $(CFLAGS) DibHeader.cpp
	
BitMapHeader.o: BitMapHeader.cpp
	$(CC) $(CFLAGS) BitMapHeader.cpp

clean:
	rm -rf *.o core.* Triangle.out



