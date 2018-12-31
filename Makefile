# Block Mover
make:
	g++ -Wall -pedantic -g moveblocks.cpp blocks.cpp search.cpp node.cpp -o moveblocks -lpopt

clean:
	rm ./moveblocks
