CC=g++

CFLAGS = -c -Wall -std=c++11

LIBS = -lGL -lX11


all: Triangle

Triangle: Triangle.o
	$(CC) Triangle.o -o Triangle.out $(LIBS)

Triangle.o: Triangle.cpp
	$(CC) $(CFLAGS) Triangle.cpp 

clean:
	rm -rf *.o core.* Triangle.out



