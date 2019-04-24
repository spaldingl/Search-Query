CC = g++
DEBUG = -g
CFLAGS = -Wall -std=gnu++11 $(DEBUG)
LFLAGS = -Wall $(DEBUG)

compile: lab11.out

lab11.out: lab11.cpp searchpp.h
	$(CC) $(CFLAGS) -o lab11.out lab11.cpp

run: lab11.out
	./lab11.out

clean:
	rm *.out
