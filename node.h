/* Nathan Gilbert
 * This is the definition for a node in the
 * search space graph.
 */
#ifndef NODE_H
#define NODE_H

#include <iomanip>
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>

using namespace std;

class Node {
private:
  /* Hold the move that propagated this state. */
  string backpath;

  /* The state of things. */
  vector<vector<char>> state;

  /* The node that lead to here. */
  Node *parent;

  /* The depth of this node. Used for DFS */
  int depth;

  /* The cost to this node. */
  int cost;

  /* The h(n) of this node. */
  int heuristic;

  /* The f(n) of this node. */
  int estimate;

public:
  // Constructors/Destructors
  Node();
  Node(vector<vector<char>> s);
  Node(vector<vector<char>> s, string path);
  Node(vector<vector<char>> s, string path, Node p);
  ~Node();

  // These are the accessor methods for the path information of this node.
  void addPath(string s);
  string getMove();

  /* Prints out a neat representation of this node. */
  void prettyPrint(int, bool, bool);

  /* Change or update this node's state.*/
  void updateState(vector<vector<char>>);
  vector<vector<char>> getState();

  Node *getParent();

  /* Accessors for various private attributes. */
  void setDepth(int);
  int getDepth();

  void setH(int);
  int getH();

  void updateF();
  int getF() const;

  void setCost(int);
  int getCost();

  /* Overloaded operator used for A* priority queue. */
  bool operator<(const Node rhs) const;
};

#endif
