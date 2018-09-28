#ifndef BFS_H
#define BFS_H

#include "edgelist.h" 
#include "vertex.h"

#define TRUE 1
#define FALSE 0

//breasth first search algorithm
void bfs(int start_vertex_idx, struct Vertex *vertices, struct Edge *edges_sorted, int numOfVertices, int numEdges);

#endif