#include <stdio.h>
#include <memory.h>

#include "bfs.h"
#include "sort.h"
#include "edgelist.h" 
#include "vertex.h"
#include "timer.h"

void printMessageWithtime(const char * msg, double time){

    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", msg);
    printf(" -----------------------------------------------------\n");
    printf("| %-51f | \n", time);
    printf(" -----------------------------------------------------\n");

}


int main(void) {

    
    const char * fname = "./datasets/test/test.txt";
    // const char * fname = "./datasets/wiki-vote/wiki-Vote.txt";
    // const char * fname = "./datasets/RMAT/RMAT22";
    
    int numOfVertices = 0;
    int numOfEdges = 0;
    struct Timer* timer = (struct Timer*) malloc(sizeof(struct Timer));

    // get |v| |e| count do we can allocate our edge array and vertex array
    loadEdgeArrayInfo(fname, &numOfVertices, &numOfEdges);
    printf("Edges : %d Vertices: %d\n", numOfEdges, numOfVertices);

    // allocate our edge array and vertex array
    struct Edge * edgeArray = newEdgeArray(numOfEdges);
    struct Edge * sortedEdgeArray = newEdgeArray(numOfEdges);
    struct Vertex * vertexArray =  newVertexArray(numOfVertices);

    // populate the edge array from file
    loadEdgeArray(fname, edgeArray);
    


    /*the function you need to optimize*/
    // radixSortEdgesBySource(sortedEdgeArray, edges, numVertices, numEdges);
    Start(timer);
    countSortEdgesBySource(sortedEdgeArray, edgeArray, numOfVertices, numOfEdges);
    Stop(timer);
    printMessageWithtime("Time Sorting (Seconds)",Seconds(timer));
    /*the function you need to optimize*/


    // if you want to check
    // printEdgeArray(edgeArray, numOfEdges); // print the edge list unsorted 
    // printEdgeArray(sortedEdgeArray, numOfEdges); 

    mapVertices(vertexArray, sortedEdgeArray, numOfVertices, numOfEdges);

    Start(timer);
    bfs(6, vertexArray, sortedEdgeArray, numOfVertices, numOfEdges);
    Stop(timer);
    printMessageWithtime("Time BFS (Seconds)",Seconds(timer));

    free(sortedEdgeArray);
    free(vertexArray);
    free(edgeArray);
    
    return 0;
}


