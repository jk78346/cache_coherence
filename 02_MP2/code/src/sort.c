#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

#include "sort.h"
#include "graph.h"
#include "graph.h"


struct Graph* radixSortEdgesBySource (struct Graph* graph){

        // printf("*** START Radix Sort Edges By Source *** \n");

    // Do counting sort for every digit. Note that instead
    // of passing digit number, exp is passed. exp is 10^i
    // where i is current digit number

    int radix = 4;  // 32/8 8 bit radix needs 4 iterations
    int P = numThreads;  // 32/8 8 bit radix needs 4 iterations
    int buckets = 256; // 2^radix = 256 buckets
    int num_edges = graph->num_edges;
    int* buckets_count = NULL;
    int inverse = 0;

    omp_set_num_threads(P);
   
    int j = 0; //1,2,3 iteration

    struct Edge* temp_sorted_edges_array = newEdgeArray(num_edges);

    buckets_count = (int*) malloc(P * buckets * sizeof(int));
    
    // sort the noraml graph for push implemention
    for(j=0 ; j < radix ; j++){
        radixSortCountSortEdgesBySource (&temp_sorted_edges_array, graph, j, buckets, buckets_count, inverse);
    }

    free(buckets_count);
    free(temp_sorted_edges_array);

    // sort inverese graph for pull implementation
    temp_sorted_edges_array = newEdgeArray(num_edges);
    buckets_count = (int*) malloc(P * buckets * sizeof(int));
    inverse = 1;


    for(j=0 ; j < radix ; j++){
        radixSortCountSortEdgesBySource (&temp_sorted_edges_array, graph, j, buckets, buckets_count, inverse);
    }

    free(buckets_count);
    free(temp_sorted_edges_array);

    return graph;

}


void radixSortCountSortEdgesBySource (struct Edge** temp_sorted_edges_array, struct Graph* graph, int radix, int buckets, int* buckets_count, int inverse){

    struct Edge* temp_edges_array_pointer = NULL;
    struct Edge** edges_array_to_be_sorted = NULL;
    int num_edges = graph->num_edges;
    int t = 0;
    int o = 0;
    int u = 0;
    int i = 0;
    int j = 0;
    int P = numThreads;  // 32/8 8 bit radix needs 4 iterations
    int t_id = 0;
    int offset_start = 0;
    int offset_end = 0;
    int base = 0;

    if(inverse == 1)
        edges_array_to_be_sorted = &graph->inverse_sorted_edges_array; 
    else
        edges_array_to_be_sorted = &graph->sorted_edges_array; 

    #pragma omp parallel default(none) shared(temp_sorted_edges_array,edges_array_to_be_sorted,radix,buckets,buckets_count,num_edges) firstprivate(t_id, P, offset_end,offset_start,base,i,j,t,u,o) 
    {
        P = omp_get_num_threads();
        t_id = omp_get_thread_num();
        offset_start = t_id*(num_edges/P);


        if(t_id == (P-1)){
            offset_end = offset_start+(num_edges/P) + (num_edges%P) ;
        }
        else{
            offset_end = offset_start+(num_edges/P);
        }
        

        //HISTOGRAM-KEYS 
        for(i=0; i < buckets; i++){ 
            buckets_count[(t_id*buckets)+i] = 0;
        }

       
        for (i = offset_start; i < offset_end; i++) {      
            u = (*edges_array_to_be_sorted)[i].src;
            t = (u >> (radix*8)) & 0xff;
            buckets_count[(t_id*buckets)+t]++;
        }


        #pragma omp barrier

       
        // SCAN BUCKETS
        if(t_id == 0){
            for(i=0; i < buckets; i++){
                 for(j=0 ; j < P; j++){
                 t = buckets_count[(j*buckets)+i];
                 buckets_count[(j*buckets)+i] = base;
                 base += t;
                }
            }
        }


        #pragma omp barrier

        //RANK-AND-PERMUTE
        for (i = offset_start; i < offset_end; i++) {       /* radix sort */
            u = (*edges_array_to_be_sorted)[i].src;
            t = (u >> (radix*8)) & 0xff;
            o = buckets_count[(t_id*buckets)+t];
            (*temp_sorted_edges_array)[o] = (*edges_array_to_be_sorted)[i];
            buckets_count[(t_id*buckets)+t]++;

        }

    }

    temp_edges_array_pointer = *temp_sorted_edges_array;
    *temp_sorted_edges_array = *edges_array_to_be_sorted;
    *edges_array_to_be_sorted = temp_edges_array_pointer;
    
}