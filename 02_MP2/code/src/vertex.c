#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "edgelist.h"
#include "vertex.h"
#include "graph.h"


struct Vertex * newVertexArray(int num_vertices){

	struct Vertex *vertexArray = (struct Vertex *)malloc ( num_vertices * sizeof(struct Vertex) ) ;

	// if ( !edgeArray )
 //    {
 //        fprintf ( stderr, "Error, malloc: %s\n", strerror ( errno )) ;
 //        exit ( EXIT_FAILURE ) ;
 //    }

    int i;
     for(i = 0; i < num_vertices; i++){

            vertexArray[i].edges_idx = 0;
            vertexArray[i].out_degree = 0;
            vertexArray[i].in_degree = 0;
     }

	return vertexArray;
}



// Populate each vertex with the corresponding position 
// of its outgoing edges. 
// Complexity: O(E)
struct Graph * mapVertices(struct Graph *graph) {
    int i;
    int vertex_id;

    for(i = 0; i < graph->num_vertices; ++i) {
         graph->vertices[i].edges_idx = NO_OUTGOING_EDGES;
    }

    // first vertex, which has outgoing edges
    vertex_id =  graph->sorted_edges_array[0].src;
    graph->vertices[vertex_id].edges_idx = 0;
    graph->vertices[vertex_id].out_degree++;
    graph->inverse_vertices[vertex_id].in_degree++;

    // process the other vertices
    for(i = 1; i < graph->num_edges; ++i) {

        vertex_id =  graph->sorted_edges_array[i].src;
        graph->vertices[vertex_id].out_degree++;
        graph->inverse_vertices[vertex_id].in_degree++;
        
        if( graph->sorted_edges_array[i].src !=  graph->sorted_edges_array[i - 1].src) {
            graph->vertices[vertex_id].edges_idx = i;
        }
    }


    for(i = 0; i < graph->num_vertices; ++i) {
         graph->inverse_vertices[i].edges_idx = NO_OUTGOING_EDGES;
    }

    // first vertex, which has outgoing edges
    vertex_id =  graph->inverse_sorted_edges_array[0].src;
    graph->inverse_vertices[vertex_id].edges_idx = 0;
    graph->vertices[vertex_id].in_degree++;
    graph->inverse_vertices[vertex_id].out_degree++;

    // process the other vertices
    for(i = 1; i < graph->num_edges; ++i) {

        vertex_id =  graph->inverse_sorted_edges_array[i].src;
        graph->vertices[vertex_id].in_degree++;
        graph->inverse_vertices[vertex_id].out_degree++;

        if( graph->inverse_sorted_edges_array[i].src !=  graph->inverse_sorted_edges_array[i - 1].src) {
             graph->inverse_vertices[vertex_id].edges_idx = i;
        }
    }


    return graph;

}