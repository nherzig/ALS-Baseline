CC=g++
CFLAGS= -Wall
LIBS=-L/usr/local/lib -lfftw3 -lfftw3f -lfftw3l
INCLUDE=-I/usr/local/include -I/usr/local/include/eigen3 -I/usr/local/include/eigen3/unsupported/

all: main

main.o: main.cpp
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) -c main.cpp -o main.o

main: main.o
	$(CC) $(CFLAGS) $(LIBS) $(INCLUDE) main.o -o main

clean: main.o
	rm main.o