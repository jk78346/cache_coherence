#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include "edgelist.h"
#include "graph.h"

int maxTwoIntegers(int num1, int num2){

        if(num1 >= num2)
                return num1;
        else
                return num2;

}

void printEdgeArray(struct Edge *edgeArray, int numOfEdges){



    int i;
    	printf("\n");
     for(i = 0; i < numOfEdges; i++){
     	printf("%d->%d\n",edgeArray[i].src, edgeArray[i].dest);
        }


}


// initialize a new edge array
struct Edge * newEdgeArray(int numOfEdges){

	struct Edge *edgeArray = (struct Edge *)malloc ( numOfEdges * sizeof(struct Edge) ) ;

	// if ( !edgeArray )
 //    {
 //        fprintf ( stderr, "Error, malloc: %s\n", strerror ( errno )) ;
 //        exit ( EXIT_FAILURE ) ;
 //    }

    int i;
     for(i = 0; i < numOfEdges; i++){

            edgeArray[i].dest = 0;
            edgeArray[i].src = 0;  
                
     }

	return edgeArray;
}


// Reads the graph in the following format:
// SRC1 DEST1
// SRC2 DEST2
// ...
// SRCn DESTn
//
// Where:
// SRCi DESTi - the i-th edge from the vertex SRCi to the vertex DESTi 
struct Graph *  loadEdgeArray(const char * fname, struct Graph* graph){

		FILE *pText;
        int size = 0, i;
        int src = 0, dest = 0;
    
        pText = fopen(fname, "r");
       
        if (pText == NULL) {
                // err(1, "open: %s", fname);
                return NULL;
        }
      

        while (1)
        {

        
        i = fscanf(pText, "%d\t%d\n", &src, &dest);
        if( i == EOF ) 
           break;

        graph->sorted_edges_array[size].src = src;
        graph->sorted_edges_array[size].dest = dest;

        graph->inverse_sorted_edges_array[size].src = dest;
        graph->inverse_sorted_edges_array[size].dest = src;
                 
        size++;

        }
        
      

        fclose(pText);

        return graph;
      
}


void   loadEdgeArrayInfo(const char * fname, int *numOfVertices, int *numOfEdges){

		FILE *pText;
        int size = 0, i;
        int src = 0, dest = 0, num_vertices = 0;

        // printf("Filename : %s \n",fname);
    
        pText = fopen(fname, "r");
       
        if (pText == NULL) {
                // err(1, "open: %s", fname);
                return;
        }
      

        while (1)
        {

        size++;
        
        i = fscanf(pText, "%d\t%d\n", &src, &dest);
        if( i == EOF ) 
           break;

        num_vertices = maxTwoIntegers(num_vertices,maxTwoIntegers(src, dest));
               
        }

     
        
        
       
        fclose(pText);

        *numOfVertices = num_vertices+1;
        *numOfEdges = size-1;

}


