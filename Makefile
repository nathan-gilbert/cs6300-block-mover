#Nathan Gilbert
#February 4, 2007
#CS 6300
#Makefile for Assignment 1.

make:
	g++ -Wall -pedantic -g moveblocks.cpp blocks.cpp search.cpp node.cpp -o moveblocks -lpopt

clean:
	rm ./moveblocks
