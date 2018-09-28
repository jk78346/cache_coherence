#ifndef SORT_H
#define SORT_H

#include "edgelist.h"
// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
void countSortEdgesBySource(struct Edge *edges_sorted, struct Edge *edges, int numVertices, int numEdges);

#endif