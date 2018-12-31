/*
 *
 *
 *
 */
#include "node.h"

Node::Node()
{
	backpath = "";
	cost = 0;
	estimate = 0;
	heuristic = 0;
	parent = NULL;
	depth = 0;
}

Node::Node(vector<vector<char> > s)
{
	backpath = "";
	state = s;
	estimate = 0;
	cost = 0;
	heuristic = 0;
	parent = NULL;
	depth = 0;
}

Node::Node(vector<vector<char> > s, string path, Node p)
{
	backpath = path;
	state = s;
	estimate = 0;
	cost = 0;
	heuristic = 0;
	parent = new Node;
	depth = 0;

	if(parent)
		*parent = p;
	else
	{
		cout << "Allocation error." << endl;
		exit( EXIT_FAILURE );
	}
}

Node::Node(vector<vector<char> > s, string path)
{
	backpath = path;
	state = s;
	cost = 0;
	estimate = 0;
	heuristic = 0;
	parent = NULL;
	depth = 0;
}

Node::~Node()
{
	backpath.clear();
	state.clear();
	parent = NULL;

	if(parent != NULL)
		delete parent;
}

void Node::addPath(string s)
{
	backpath.append(s + "\n");
}

/* Takes the total number of tables in this state, and two bools
 * final = Is this the final print of a solution?
 * heur = Should this print heuristic informatin? (Only used with A*)
 */
void Node::prettyPrint(int tables, bool final, bool heur)
{
	int max_height = 0;
	int level = 0;
	string buf = "";
	vector<string> columns;

	/* Finding the maximum height. */
	for(int i = 1; i < tables+1;i++)
		if(max_height < (int)state[i].size())
			max_height = state[i].size();

	for(int i = 0; i < tables; i++)
		buf.append("_ ");

	columns.push_back(buf);
	buf.clear();

	while(level < max_height)
	{
		for(int i = 1; i < tables+1;i++)
		{
			if(state[i].size() > (unsigned)level)
			{
				string x(1,state[i][level]);
				buf.append(x + " ");
			}
			else
				buf.append("  ");
		}
		
		columns.push_back(buf);
		buf.clear();
		level++;
	}

	if(!final)
		if(!heur)
			cout << "Expanding node: \n" << endl;	
		else
			cout << "Expanding node: (g=" << cost << ", h=" << heuristic << ", f=" << estimate << "):\n"  << endl;	
	else
		cout << "Solution node: \n" << endl;	

	for(int i = (int)columns.size() - 1; i >= 0;i--)
		cout << setw(30) << columns[i] << endl;
}

/*
 * Many accessor methods follow, they do just as they
 * say. Either set's or get's. 
 */
vector<vector<char> > Node::getState()
{
	return state;
}

Node *Node::getParent()
{
	return parent;
}

void Node::updateState(vector<vector<char> > s)
{
	state = s;
}

string Node::getMove()
{
	return backpath;
}

void Node::setDepth(int d)
{
	depth = d;
}

void Node::setH(int h)
{
	heuristic = h;	
}

int Node::getH()
{
	return heuristic;
}

void Node::updateF()
{
	estimate = cost + heuristic;
}

int Node::getF() const
{
	return estimate;
}

int Node::getDepth()
{
	return depth;
}

void Node::setCost(int c)
{
	cost = c;
}

int Node::getCost() 
{
	return cost;
}

/* Less than defined in terms of greater-than. A quirck of Priority Queues in C++ */
bool Node::operator< (const Node rhs) const
{
    return (estimate > rhs.getF());
}
