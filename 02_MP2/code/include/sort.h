#ifndef SORT_H
#define SORT_H

#include "edgelist.h"
#include "graph.h"
// Order edges by id of a source vertex, 
// using the Counting Sort
// Complexity: O(E + V)
struct Graph* radixSortEdgesBySource (struct Graph* graph);
void radixSortCountSortEdgesBySource (struct Edge** sorted_edges_array, struct Graph* graph, int radix, int buckets, int* buckets_count, int inverse);


extern int numThreads;

#endif