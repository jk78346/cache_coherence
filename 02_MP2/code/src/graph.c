#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "graph.h"
#include "edgelist.h"
#include "vertex.h"


void   loadGraphInfo(const char * fname, struct Graph *graph){

		 loadEdgeArrayInfo(fname, &graph->num_vertices, &graph->num_edges);

}

// initialize a new graph from file
struct Graph * newGraph(const char * fname){

	int i;

	struct Graph* graph = (struct Graph*) malloc(sizeof(struct Graph));

	loadGraphInfo(fname, graph);

	graph->parents  = (int*) malloc( graph->num_vertices *sizeof(int));

    graph->vertices = newVertexArray(graph->num_vertices);
    graph->sorted_edges_array = newEdgeArray(graph->num_edges);

    graph->inverse_vertices = newVertexArray(graph->num_vertices);
    graph->inverse_sorted_edges_array = newEdgeArray(graph->num_edges);

    for(i = 0; i < graph->num_vertices; i++){
        graph->parents[i] = -1;  
    }

    graph->iteration = 0;
    graph->processed_nodes = 0;

	return graph;
}


void freeGraph(struct Graph *graph){

    free(graph->vertices);
    free(graph->sorted_edges_array);

    free(graph->inverse_vertices);
    free(graph->inverse_sorted_edges_array);

	free(graph);
}