#ifndef VERTEX_H
#define VERTEX_H

#include "graph.h"

#define NO_OUTGOING_EDGES -1

struct Vertex{
    int edges_idx; // index of the first outgoing edge
    int out_degree;
    int in_degree;
};

struct Vertex * newVertexArray(int num_vertices);
struct Graph * mapVertices(struct Graph *graph);

#endif