/*
 * Nathan Gilbert
 * Implementation of several useful functions for this project.
 * Nearly everything in this file is used in parsing the input files and
 * error checking their syntax. A few helper functions that are used throughout
 * the project, such as ``min'' are also defined here.
 */
#include "blocks.h"

// Simply function to return the minimum of two functions.
int min(int x, int y) { return (x < y ? x : y); }

/*
 * This reads in the input file, tests for correct syntax, returns true if
 * everything goes well, else false.
 *
 * Also creates the initial and goal states, but does not create their nodes.
 * This is done in moveblocks.cpp.
 */
bool readInit(const char *inFile, int &tables, int &blocks,
        vector<vector<char> > &is, vector<vector<char> > &gs) {
  ifstream in;
  string line, buf;
  int table = -1;
  int goal_blocks = 0;
  bool goal = false, found = false;
  vector<char> tokens, match;

  in.open(inFile, ios::in);

  /* Did the file open correctly? */
  if (in.fail())
    return false;

  // Read the first line.
  getline(in, line);

  if (line.substr(0, 13) != "# of tables: ")
    return false;

  /* Check this for improper numbers. */
  tables = atoi(line.substr(13, line.length() - 1).c_str());

  // Checking for the allowable number of tables.
  if (tables > 10 || tables < 0)
    return false;

  getline(in, line);

  /* Catch the initial state. */
  if (line != "INITIAL STATE")
    return false;

  while (!in.eof()) {
    getline(in, line);

    /* Catch the goal state. */
    if (line == "GOAL STATE") {
      goal = true;
      break;
    }

    table = atoi(line.substr(line.find_last_of("table") + 1, line.find(":") + 1)
                     .c_str());

    if (table > tables)
      return false;

    buf = line.substr(line.find(":") + 1, line.length() - 1);
    tokenize(buf, tokens);

    for (unsigned i = 0; i < tokens.size(); i++) {
      is[table].push_back(tokens[i]);
      match.push_back(tokens[i]);
      blocks++;
    }
  }

  // Checking for the correct range of blocks.
  if (blocks > 30 || blocks < 0)
    return false;

  // Start collecting the goal state.
  while (!in.eof()) {
    getline(in, line);
    table = atoi(line.substr(line.find_last_of("table") + 1, line.find(":") + 1)
                     .c_str());

    if (table > 30)
      return false;

    buf = line.substr(line.find(":") + 1, line.length() - 1);

    if (buf == "")
      continue;

    tokenize(buf, tokens);

    for (unsigned i = 0; i < tokens.size(); i++) {

      // Checking for cases in which a block is used more in the goal state than
      // in the initial states.
      found = false;
      vector<char>::iterator iter;
      for (iter = match.begin(); iter != match.end(); iter++) {
        if (*iter == tokens[i]) {
          match.erase(iter);
          found = true;
          break;
        }
      }

      // If the block in
      if (!found)
        return false;

      // If everything is fine, add the block to the goal state. ::
      gs[table].push_back(tokens[i]);
      goal_blocks++;
    }
  }

  in.close();

  if (!goal)
    return false;

  if (goal_blocks > blocks)
    return false;

  return true;
}

/*
 * This function handles printing the inital and goal states after they have
 * been correctly parsed and read from file. This function is only called once
 * per run.
 */
void printInit(vector<vector<char> > is, vector<vector<char> > gs) {
  string buf = "";
  int b = 0;
  int level = 0;
  int goal_blocks = 0;
  vector<string> columns;

  for (int i = 0; i < tables; i++)
    buf.append("_ ");

  columns.push_back(buf);
  buf.clear();

  // Check to make sure all blocks are accounted for.
  while (b != blocks) {
    for (int i = 1; i < tables + 1; i++) {
      if (is[i].size() > (unsigned)level) {
        string x(1, is[i][level]);
        buf.append(x + " ");
        b++;
      } else
        buf.append("  ");
    }

    columns.push_back(buf);
    buf.clear();
    level++;
  }

  cout << "\n\tInitial State:\n" << endl;

  for (int i = (int)columns.size() - 1; i >= 0; i--)
    cout << setw(30) << columns[i] << endl;

  // Determining how many goal blocks there are.
  for (int i = 1; i < tables + 1; i++)
    goal_blocks += (int)gs[i].size();

  cout << "\n\tGoal State:\n" << endl;

  // Reinitialize variables.
  columns.clear();
  b = 0;
  level = 0;

  for (int i = 0; i < tables; i++)
    buf.append("_ ");

  columns.push_back(buf);
  buf.clear();

  while (b != goal_blocks) {
    for (int i = 1; i < tables + 1; i++) {
      if (gs[i].size() > (unsigned)level) {
        string x(1, gs[i][level]);
        buf.append(x + " ");
        b++;
      } else
        buf.append("  ");
    }

    columns.push_back(buf);
    buf.clear();
    level++;
  }

  for (int i = (int)columns.size() - 1; i >= 0; i--)
    cout << setw(30) << columns[i] << endl;
}

/*
 * A string tokenizer for C++.
 */
void tokenize(string s, vector<char> &tokens) {
  tokens.clear();

  for (unsigned i = 0; i < s.length(); i++) {
    if (s[i] == ' ')
      continue;
    else
      tokens.push_back(s[i]);
  }
}
