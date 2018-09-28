#ifndef VERTEX_H
#define VERTEX_H

#define NO_OUTGOING_EDGES -1

struct Vertex{
    int edges_idx; // index of the first outgoing edge
};

struct Vertex * newVertexArray(int numOfVertices);
void mapVertices(struct Vertex *vertices, struct Edge *edges_sorted, int numOfVertices,  int numEdges);

#endif