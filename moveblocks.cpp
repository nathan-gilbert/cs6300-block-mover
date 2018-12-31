/*
 * Nathan Gilbert
 * This is the main driver of the blocks program.
 */
#include <iostream>
#include <stdlib.h>
#include <popt.h>
#include <vector>
#include "blocks.h"
#include "search.h"

using namespace std;

/*
 * A few useful constants. 
 */
#define DEFAULT_VALUE	-1

/*
 * Various global values which determine what commandline arguments have 
 * been read in.
 */
int 	args_needed = 1;
int 	tables = 0;
int 	blocks = 0;
int	bfs = false;
int	dfs = false;
int 	alth = false;
int 	stats = false;
int 	astar = false;
int 	trace = false;
int	debug = false;
int	depth_value = DEFAULT_VALUE;
int 	nodes_expanded = 0;
int 	nodes_generated = 0;
int 	solution_length = 0;
vector<vector<char> > goal(11);

char	*Usage  = "[OPTIONS...] <startup_file>";
struct poptOption OptionsTable[] = {
    { "bfs", 'b', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &bfs, 0, "Use breadth-first-search.", NULL },
    { "dfs", 'd', POPT_ARG_INT | POPT_ARGFLAG_ONEDASH, &depth_value, 0, "Use depth-first-search, the integer depth limit following the -dfs flag, indicates the maximum number of moves to be explored along any path from the start node.", NULL }, 
    { "astar", 'a', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &astar, 0, "Use the A* search algorithm.", NULL },
    { "alth", 'h', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &alth, 0, "Use an alternate heuristic function, cannot be used with bfs or dfs.", NULL},
    { "trace", 't', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &trace, 0, "List each node in the search space as it is expanded, along with relevant information about path cost and heuristic function.", NULL},
    { "stats", 's', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &stats, 0, "Output information about the efficency of the search.", NULL},
    { "debug", 'g', POPT_ARG_NONE | POPT_ARGFLAG_ONEDASH, &debug, 0, "Print out the path from solution to start node and all relevant heuristic information.", NULL},
	 POPT_AUTOHELP{ NULL, 0, 0, NULL, 0 }
};

int main(int argc, char *argv[])
{
	//Boiler plate arg processing code.
   poptContext PoptContext;
   int         PoptReturnCode;
   const char  **args;
   int         arg;
   int			argpt = 0;
	float 		search_efficiency = 0.0;
	vector<vector<char> > initial(11);

   PoptContext = poptGetContext (NULL, argc, ( const char **) argv, OptionsTable, POPT_CONTEXT_POSIXMEHARDER);
   poptSetOtherOptionHelp (PoptContext, Usage);
   PoptReturnCode = poptGetNextOpt (PoptContext);

	if (PoptReturnCode != -1) 
	{
		fprintf ( stderr, "%s: %s\n",
		poptBadOption ( PoptContext, POPT_BADOPTION_NOALIAS ),
		poptStrerror ( PoptReturnCode ) );
		return ( EXIT_FAILURE );
   }
 		 
	args = poptGetArgs (PoptContext);
  	arg = 0;
    
	while ( ( args != NULL ) && ( args[arg] != NULL ) )
  	{
		arg++;
	} 
    
	if ( arg != args_needed )
  	{
		poptPrintHelp ( PoptContext, stderr, 0 );
		return ( EXIT_FAILURE );
   }

	//Only one search can be performed at a time. 
	if ( (depth_value != DEFAULT_VALUE && (bfs || astar)) || (bfs && astar) )
  	{
		cout << "Only one search method can run at a time.\nPlease try again." << endl;
		return ( EXIT_FAILURE );
   }
	else if(alth && (bfs || depth_value != DEFAULT_VALUE))
	{
		cout << "The alternate heuristic function is only relevant for A*.\nPlease try again." << endl;
		return ( EXIT_FAILURE );
	}

	//If the command line arguments are OK, let's try to read the input file.	
	if(!readInit(args[argpt++], tables, blocks, initial, goal))
	{
		cout << "\nInitial file error. " << endl;
		cout << "There could be several reasons for this, make sure file syntax is correct and that the file actually exists. " << endl;
		cout << "Please try again. \n" << endl;
		return ( EXIT_FAILURE );
	}

	/* Initial printouts. */
	cout << "\nNumber of tables: " << tables << endl;
	printInit(initial, goal);

	//Create the start node.
	Node init(initial);

	//Execute the desired search.
	if(bfs == 1)
	{
		cout << "\n\t--| Breadth First Search |--\n" << endl;
		if(!breadthFirst(init))
			cout << "\n-->No Solution Found" << endl;
	}
	else if(depth_value != DEFAULT_VALUE)
	{
		cout << "\n\t--| Depth First Search |--\n" << endl;
		if(!depthFirst(init, depth_value))
			cout << "\n-->No Solution Found" << endl;
	}
	else if(astar == 1)
	{
		cout << "\n\t--| A* Search |--" << endl;

		if(alth)
			cout << "\t..using alternate heuristic.\n" << endl;
		else
			cout << endl;

		if(!aStarSearch(init))
			cout << "\n-->No Solution Found" << endl;
	}
	else
	{
		cout << "\n\t--| A* Search |--\n" << endl;
		if(!aStarSearch(init))
			cout << "\n-->No Solution Found" << endl;
	}


	if(stats)
	{
		//Prinout the desired information.
		cout << "\nNodes expanded: " << nodes_expanded << endl;
		cout << "Nodes generated: " << nodes_generated << endl;
		cout << "Nodes in solution: " << solution_length << endl;

		if(nodes_expanded != 0)
		{
			search_efficiency = ((float)solution_length / nodes_expanded)*100; 
			cout << "Search Efficency: " << search_efficiency << "%" << endl;
		}
		else
			cout << "Search Efficency: 100%" << endl;

	}

	/* Normal exit after execution of the program. */
   return ( EXIT_SUCCESS );	
}

