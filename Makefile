CC = g++
CFLAGS = -g -Wall

output: main.o matrix.o
	$(CC) -pthread main.o matrix.o -o output

main.o: main.cpp
	$(CC) -pthread -c main.cpp

matrix.o: matrix.cpp matrix.h
	$(CC) -pthread -c matrix.cpp 

clean:
	rm *.o output
