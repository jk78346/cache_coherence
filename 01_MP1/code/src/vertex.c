#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "edgelist.h"
#include "vertex.h"


struct Vertex * newVertexArray(int numOfVertices){

	struct Vertex *vertexArray = (struct Vertex *)malloc ( numOfVertices * sizeof(struct Vertex) ) ;

	// if ( !edgeArray )
 //    {
 //        fprintf ( stderr, "Error, malloc: %s\n", strerror ( errno )) ;
 //        exit ( EXIT_FAILURE ) ;
 //    }

    int i;
     for(i = 0; i < numOfVertices; i++){

            vertexArray[i].edges_idx = 0;
          
     }

	return vertexArray;
}



// Populate each vertex with the corresponding position 
// of its outgoing edges. 
// Complexity: O(E)
void mapVertices(struct Vertex *vertices, struct Edge *edges_sorted, int numOfVertices, int numEdges) {
    int i;
    int vertex_id;
    for(i = 0; i < numOfVertices; ++i) {
        vertices[i].edges_idx = NO_OUTGOING_EDGES;
    }

    // first vertex, which has outgoing edges
    vertex_id = edges_sorted[0].src;
    vertices[vertex_id].edges_idx = 0;

    // process the other vertices
    for(i = 1; i < numEdges; ++i) {
        if(edges_sorted[i].src != edges_sorted[i - 1].src) {
            vertex_id = edges_sorted[i].src;
            vertices[vertex_id].edges_idx = i;
        }
    }
}