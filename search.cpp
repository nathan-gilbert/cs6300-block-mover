/*
 * Nathan Gilbert
 * This set of functions implements all the generic search algorithms
 * I decided to split up the algorithm into separate functions to promote ease of
 * reading, I felt that having one ``generic'' search function, parameterized to
 * morph into BFS/DFS/A* upon the values it was feed to be somewhat hard to read
 * and follow.
 *
 */

#include "search.h"

/*
 * This function determines if the given node is a goal state.
 * Returns true if it is, else false.
 */
bool check_goal(Node poss)
{

    //Determine relevant tables in the goal state.
    vector<int> relevant_tables;

    //Are all the node's tables the same as the goal's?
    int true_count = 0;

    //Are all the blocks on the node's table the same as the goal's?
    int true_count_table = 0;

    for(int i = 1;i < (int)goal.size();i++)
        if(goal[i].size() > 0)
            relevant_tables.push_back(i);

    //Compare blocks that rest on the non-empty tables in teh goal state.
    for(int i = 0;i < (int)relevant_tables.size();i++)
    {
        if(goal[relevant_tables[i]].size() != poss.getState()[relevant_tables[i]].size())
            return false;

        for(int j = 0; j < (int)goal[relevant_tables[i]].size();j++)
        {
            if(goal[relevant_tables[i]][j] == poss.getState()[relevant_tables[i]][j])
            {
                true_count_table++;
            }
        }

        //Checks if the tables between node and goal are matching.
        if(true_count_table == (int)goal[relevant_tables[i]].size())
        {
            true_count++;
            true_count_table = 0;
        }
        else
            return false;
    }

    //Checks to see if all tables match.
    if(true_count == (int)relevant_tables.size())
        return true;

    return false;
}

/*
 * Prints the final solution if one exists.
 */
void printSolution(Node n)
{
    Node *p;
    vector<string> backPath;
    backPath.push_back(n.getMove());

    if(debug)
        n.prettyPrint(tables, false, true);

    p = n.getParent();

    //Walk the pointers back up to the initial node.
    while(p != NULL)
    {
        solution_length++;
        backPath.push_back(p->getMove());

        if(debug)
            p->prettyPrint(tables, false, true);

        p = p->getParent();
    }

    cout << "\n\nSolution Found!\n";
    for(int i = (int)backPath.size()-1;i >= 0;i--)
    {
        cout << backPath[i] << endl;
    }
}

/*
 * Generates all successors of node c and put them in vector s.
 */
void getSuccessors(Node c, vector<Node> &s)
{
    vector<vector<char> > main;
    vector<vector<char> > temp;
    string move = "Moved block ";
    char num[10];
    main = c.getState();

    //Each successor has one string that hold what move
    //has just taken place.
    //This helps backtrack the steps after a solution
    //is found.
    for(int i = 1; i < tables+1;i++)
    {
        if(!main[i].empty())
        {
            /* Converting char to a string. */
            string x(1, main[i].back());

            for(int j = 1; j < tables+1;j++)
            {
                if(i == j)
                    continue;

                /* Getting the block character from the table. */
                move.append(x + " from table: ");
                temp = main;
                temp[i].pop_back();
                temp[j].push_back(x[0]);

                /* Converting ints to strings. */
                sprintf(num, "%d", i);
                move.append(num);
                sprintf(num, "%d", j);
                move.append(" to table: ");
                move.append(num);

                /* Adding node to successor list. */
                Node succ(temp, move, c);
                s.push_back(succ);
                move = "Moved block ";
                temp.clear();
                nodes_generated++;
            }
        }
    }
}

/*
 * Breadth First Search
 */
bool breadthFirst(Node init)
{
    //The open_list.
    queue<Node> open_list;

    //A list to hold successors of a node.
    vector<Node> succ;

    //This current generated node.
    Node current;

    //I do a quick check to see if the initial state is also a
    //goal state. This doesn't have to be done here, but could
    //wait till it's generated.
    if(check_goal(init))
    {
        printSolution(init);
        return true;
    }

    open_list.push(init);

    while(!open_list.empty())
    {
        //Generate a node.
        current = open_list.front();
        open_list.pop();

        if(trace)
            current.prettyPrint(tables, false, false);

        //Get it's successors.
        getSuccessors(current, succ);
        nodes_expanded++;

        //Check successors, then put them on the open list.
        for(int i = 0; i < (int)succ.size();i++)
        {
            if(check_goal(succ[i]))
            {
                printSolution(succ[i]);
                return true;
            }

            open_list.push(succ[i]);
        }

        //Don't need this list anymore, clear it and get ready
        //for the next bunch.
        succ.clear();
    }

    return false;
}

/*
 * Depth-Limited Depth First Search
 */
bool depthFirst(Node init, int depth)
{
    //Much the same as BFS, except a stack is used to implement the
    //open list.
    stack<Node> open_list;
    vector<Node> succ;
    Node current;

    if(check_goal(init))
    {
        printSolution(init);
        return true;
    }

    //Set the depth of the root node as zero.
    init.setDepth(0);
    open_list.push(init);

    while(!open_list.empty())
    {
        current = open_list.top();

        if(trace)
            current.prettyPrint(tables, false, false);

        open_list.pop();

        //Generate the successors.
        getSuccessors(current, succ);
        nodes_expanded++;

        /* Updating depths for each of the successors */
        for(int i = 0; i < (int)succ.size();i++)
        {
            succ[i].setDepth(current.getDepth()+1);
        }

        for(int i = 0; i < (int)succ.size();i++)
        {
            if(check_goal(succ[i]) && succ[i].getDepth() <= depth)
            {
                printSolution(succ[i]);
                return true;
            }

            //If a successors depth is the same as the depth, no need
            //to push it on the stack, none of it's successor can be
            //checked for the goal.
            if(succ[i].getDepth() < depth)
                open_list.push(succ[i]);
        }

        succ.clear();
    }

    return false;
}


/*
 * This returns the defualt f() value for Node n.
 */
void getDefaultF(Node &n)
{
    /*
     * Getting the tables from the goal state that actually contain blocks.
     */
    vector<int> relevant_tables;
    int h = 0;
    int node_table_size = 0;
    int goal_table_size = 0;

    /* Find all tables in goal state to make it
     * easier to search through node state space.
     */
    for(int i = 1;i < (int)goal.size();i++)
        if(goal[i].size() > 0)
            relevant_tables.push_back(i);

    //i = table number in goal state.
    for(int i = 0;i < (int)relevant_tables.size();i++)
    {
        node_table_size = (int)n.getState()[relevant_tables[i]].size();
        goal_table_size = (int)goal[relevant_tables[i]].size();

        /* Break this into cases of who has the most blocks. */
        if(node_table_size == goal_table_size)
        {
            //j = position on table, where j = 0 is the bottom.
            for(int j = 0; j < node_table_size; j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += (goal_table_size - j);
                }
            }
        }
        else if(node_table_size < goal_table_size)
        {
            //If the node table has less blocks than the goal,
            //we still need to add values to the heuristics for the
            //blank spaces in the node's table that correspond to
            //non-blank spaces in the goal's table.
            //
            //This is done in the following code, and likewise in the else
            //case for when the oppisite is true.
            int j;
            for(j = 0; j < node_table_size; j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += (goal_table_size - j);
                }
            }

            for(int k = j; k < goal_table_size; k++)
            {
                h += (goal_table_size - k);
            }
        }
        else
        {
            int j;
            for(j = 0; j < goal_table_size; j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += (goal_table_size - j);
                }
            }

            for(int k = j; k < goal_table_size; k++)
            {
                h += (goal_table_size - k);
            }
        }
    }

    //Set the node's h value.
    n.setH(h);
    //Update the estimate heuristic.
    n.updateF();
}

/*
 * The following function generates the alternate heuristic.
 * It essentially keeps a tally of every out of place block in the state.
 * This is equivalent to finding an optimal solution to the relaxed problem which allows
 * one to move blocks anywhere.
 */
void getAltF(Node &n)
{
    vector<int> relevant_tables;
    int h = 0;
    int node_table_size = 0;
    int goal_table_size = 0;

    //Which tables in the goal state do we care about?
    for(int i = 1;i < (int)goal.size();i++)
        if(goal[i].size() > 0)
            relevant_tables.push_back(i);

    //i = table number in goal state.
    for(int i = 0;i < (int)relevant_tables.size();i++)
    {
        node_table_size = (int)n.getState()[relevant_tables[i]].size();
        goal_table_size = (int)goal[relevant_tables[i]].size();

        //Again break this summation into cases so that we don't get
        //index bounds errors when one table is larger than the other.
        if(node_table_size == goal_table_size)
        {
            //j = position on table, where j = 0 is the bottom.
            for(int j = 0; j < node_table_size; j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += 1;
                }
            }
        }
        else if(node_table_size < goal_table_size)
        {
            int j;
            for(j = 0;j < node_table_size;j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += 1;
                }
            }

            for(int k = j; k < goal_table_size; k++)
            {
                h += 1;
            }
        }
        else
        {
            int j;

            for(j = 0; j < goal_table_size; j++)
            {
                if(goal[relevant_tables[i]][j] != n.getState()[relevant_tables[i]][j])
                {
                    h += 1;
                }
            }

            for(int k = j; k < node_table_size; k++)
            {
                h += 1;
            }
        }
    }

    //Set the node's h value.
    n.setH(h);
    //Update the estimate heuristic.
    n.updateF();
}

/*
 * A* Search Algorithm
 */
bool aStarSearch(Node init)
{
    priority_queue<Node> open_list;
    vector<Node> succ;
    Node current;

    init.setCost(0);

    //Which heuristic to use?
    if(!alth)
        getDefaultF(init);
    else
        getAltF(init);

    open_list.push(init);

    while(!open_list.empty())
    {
        current = open_list.top();
        open_list.pop();

        getSuccessors(current, succ);

        /* Updating depths for successors */
        for(int i = 0; i < (int)succ.size();i++)
        {
            succ[i].setCost(current.getCost()+1);

            //Which heuristic to use.
            if(!alth)
                getDefaultF(succ[i]);
            else
                getAltF(succ[i]);
        }

        nodes_expanded++;

        //Check for goal state after it has been expanded.
        if(check_goal(current))
        {
            printSolution(current);
            return true;
        }

        if(trace)
        {
            current.prettyPrint(tables, false, true);
            cout << endl;
        }

        for(int i = 0; i < (int)succ.size();i++)
        {
            open_list.push(succ[i]);
        }

        succ.clear();
    }

    return false;
}

