#ifndef GRAPH_H
#define GRAPH_H


#include "edgelist.h"
#include "vertex.h"

struct Graph{

	int num_edges;
	int num_vertices;

	int * parents; // array to hold parent of nodes when you are doing BFS
	int iteration; // collects states when running BFS which iteration you are in
	int processed_nodes; // collect number of visited nodes in the graph
		
	struct Vertex* vertices;
	struct Edge* sorted_edges_array; // sorted edge array
	
	// for the pull approach you need the mapping of the inverse vertex
	struct Vertex* inverse_vertices;
	struct Edge* inverse_sorted_edges_array; // sorted edge array

};


void   loadGraphInfo(const char * fname, struct Graph *graph);
// initialize a new graph from file
struct Graph * newGraph(const char * fname);
void freeGraph(struct Graph *graph);




#endif