CC=g++

LIBS=-lsfml-window -lsfml-graphics -lsfml-system

CFLAGS=-std=c++11 -O2 -Wall

2048: resourceHandler.o tile.o board.o game.o main.o
	$(CC) $(CFLAGS) $^ -o $@ $(LIBS)

.SUFFIXES: .cpp .o

.cpp.o:
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY: clean

clean:
	rm -rf *.o 2048