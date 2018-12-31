# CS 6300 - Artificial Intelligence
## Assignment 1
## February 13, 2007

## Preliminaries

There is a test script, `tester.py`, which I wrote to generate test files for
this project. Feel free to use it.

## To compile

Assuming you are in the directory with the source code and corresponding Makefile:

```sh
% make
```

## To run

```sh
% ./moveblocks [args...] test-file
```

## To get a list of command-line arguments

```sh
 % ./moveblocks -h
```

## Testing

To run the tester.py program in order to generate test files:

```sh
% chmod +x ./tester.py
% ./tester.py <num_blocks> <num_tables>
```

G++ versions compiled under:

* gcc version 4.1.0 (SUSE Linux)
* gcc version 3.4.5 20051201 (Red Hat 3.4.5-2)

