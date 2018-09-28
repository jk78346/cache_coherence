#include <stdio.h>
#include <stdlib.h>
#include <errno.h>


#include "edgelist.h" 
#include "vertex.h"
#include "bfs.h"


void bfs(int start_vertex_idx, struct Vertex *vertices, struct Edge *edges_sorted, int numOfVertices, int numEdges) {

    int *queue = (int*)malloc(numOfVertices*sizeof(int));
    // indicates, if vertex already been enqueued into the FIFO queue
    int *enqueued = (int*)malloc(numOfVertices*sizeof(int));
    
    int i;
    // mark all vertices as not enqueued yet
    for(i = 0; i < numOfVertices; ++i) {
        enqueued[i] = FALSE;
    }
    // mark all vertices as not enqueued yet
   
    int head;            // head of the FIFO queue
    int tail;            // tail of the FIFO queue
    int vertex_idx;      // index of dequeued vertex
    int edge_idx;        // used for iteration over the outgoing edges
    int new_vertex_idx;  // index of the vertex, which should be visited
    
    // enqueue the index of the start vertex
    head = 0;
    queue[head] = start_vertex_idx;
    enqueued[start_vertex_idx] = TRUE;
    tail = 1;
    
    // while queue is not empty
    while(head < tail) {
        
        // dequeue the next vertex
        vertex_idx = queue[head];
        head++;
        
        // printf("Visiting vertex: %d\n", vertex_idx);
        
        // process the outgoing edges
        edge_idx = vertices[vertex_idx].edges_idx;
        
        if(edge_idx == NO_OUTGOING_EDGES) {
            // vertex doesn't have outgoing edges
            continue;
        }
            
        // iterate over the outgoing edges
        while(edges_sorted[edge_idx].src == vertex_idx) {
            
            // destination vertex id
            new_vertex_idx = edges_sorted[edge_idx].dest;
            
            // if the destination vertex is not yet enqueued
            if(!enqueued[new_vertex_idx]) {
                
                // add the destination vertex to the queue 
                queue[tail] = new_vertex_idx;
                tail++;
                enqueued[new_vertex_idx] = TRUE;
            }
            
            edge_idx++;
        }
    }

    printf(" -----------------------------------------------------\n");
    printf("| %-51d | \n", tail);
    printf(" -----------------------------------------------------\n");
}