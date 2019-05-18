# CPSC 351 Makefile for Assignment 2

# Creating object code
output: main.o
	g++ main.o -o output

# Used to compile the cpp files and header files
main.o: main.cpp process.h memblock.h
	g++ -c main.cpp

# Removes files
clean:
	rm *.o output
