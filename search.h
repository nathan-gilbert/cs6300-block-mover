/*
 * Nathan Gilbert
 * Search Algorithm implementations, see search.cpp for more information.
 *
 */

#ifndef SEARCH_H
#define SEARCH_H

#include "blocks.h"

bool check_goal(Node);
void printSolution(Node);
void getSuccessors(Node, vector<Node> &);
bool breadthFirst(Node i);
bool depthFirst(Node i, int d);
void getDefaultF(Node &);
void getAltF(Node &);
bool aStarSearch(Node i);

/* Globals */
extern vector<vector<char> > goal;
extern int depth_value;
extern int blocks;
extern int tables;
extern int nodes_expanded;
extern int nodes_generated;
extern int solution_length;
extern int trace;
extern int alth;
extern int debug;

#endif
