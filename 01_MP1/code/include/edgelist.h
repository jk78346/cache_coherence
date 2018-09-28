#ifndef EDGELIST_H
#define EDGELIST_H

struct Edge{
    int src;   	  // id of a source vertex
    int dest;     // id of a destination vertex
};

struct Edge * newEdgeArray(int numOfEdges);
void   loadEdgeArray(const char * fname, struct Edge *edgeArray);
void   loadEdgeArrayInfo(const char * fname, int *numOfVertices, int *numOfEdges);
int maxTwoIntegers(int num1, int num2);
void printEdgeArray(struct Edge *edgeArray, int numOfEdges);

#endif