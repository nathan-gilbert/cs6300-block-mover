/*
 * A set of helper functions for this project. See the blocks.cpp for more
 * information.
 */

#ifndef BLOCKS_H
#define BLOCKS_H

#include "node.h"
#include <fstream>
#include <iomanip>
#include <iostream>
#include <queue>
#include <stack>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

bool readInit(const char *, int &, int &, vector<vector<char>> &,
              vector<vector<char>> &);
void tokenize(string, vector<char> &);
void printInit(vector<vector<char>>, vector<vector<char>>);
int min(int, int);

extern vector<vector<char>> goal;
extern int blocks;
extern int tables;

#endif
