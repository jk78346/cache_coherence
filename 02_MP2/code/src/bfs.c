#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <omp.h>

#include "edgelist.h" 
#include "vertex.h"
#include "graph.h"
#include "bfs.h"
#include "arrayQueue.h"
#include "bitmap.h"
#include "timer.h"



// breadth-first-search(graph, source)
//  sharedFrontierQueue ← {source}
//  next ← {}
//  parents ← [-1,-1,. . . -1]
//      while sharedFrontierQueue 6= {} do
//          top-down-step(graph, sharedFrontierQueue, next, parents)
//          sharedFrontierQueue ← next
//          next ← {}
//      end while
//  return parents

void breadthFirstSearchGraphPush(int source, struct Graph* graph){

    
    struct Timer* timer = (struct Timer*) malloc(sizeof(struct Timer));
    struct Timer* timer_inner = (struct Timer*) malloc(sizeof(struct Timer));
    double inner_time = 0;
    struct ArrayQueue* sharedFrontierQueue = newArrayQueue(graph->num_vertices);

    int P = numThreads;
    int nf = 0; // number of vertices in sharedFrontierQueue
    
    struct ArrayQueue** localFrontierQueues = (struct ArrayQueue**) malloc( P * sizeof(struct ArrayQueue*));
    

   int i;
   for(i=0 ; i < P ; i++){
        localFrontierQueues[i] = newArrayQueue(graph->num_vertices);
        
   }

    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Starting Breadth First Search (PUSH)");
    printf(" -----------------------------------------------------\n");
    printf("| %-51d | \n", source);
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15s | %-15s | \n", "Iteration", "Nodes", "Time (Seconds)");
    printf(" -----------------------------------------------------\n");

    if(source < 0 && source > graph->num_vertices){
        printf(" -----------------------------------------------------\n");
        printf("| %-51s | \n", "ERROR!! CHECK SOURCE RANGE");
        printf(" -----------------------------------------------------\n");
        return;
    }

    Start(timer_inner);
    enArrayQueue(sharedFrontierQueue, source);
    graph->parents[source] = source;  
    Stop(timer_inner);
    inner_time +=  Seconds(timer_inner);

    
    printf("| TD %-12d | %-15d | %-15f | \n",graph->iteration++, ++graph->processed_nodes , Seconds(timer_inner));

    Start(timer);
    while(!isEmptyArrayQueue(sharedFrontierQueue)){ // start while 
       
            Start(timer_inner);       
            nf = topDownStepGraphCSR(graph, sharedFrontierQueue,localFrontierQueues);
            slideWindowArrayQueue(sharedFrontierQueue);
            Stop(timer_inner);

            //stats collection
            inner_time +=  Seconds(timer_inner);
            graph->processed_nodes += nf;
            printf("| TD %-12d | %-15d | %-15f | \n",graph->iteration++, nf, Seconds(timer_inner));

    } // end while
    Stop(timer);
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15d | %-15f | \n","No OverHead", graph->processed_nodes, inner_time);
    printf(" -----------------------------------------------------\n");
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15d | %-15f | \n","total", graph->processed_nodes, Seconds(timer));
    printf(" -----------------------------------------------------\n");

    for(i=0 ; i < P ; i++){
        freeArrayQueue(localFrontierQueues[i]);     
    }
    free(localFrontierQueues);
    freeArrayQueue(sharedFrontierQueue);
    free(timer);
    free(timer_inner);
}


// top-down-step(graph, sharedFrontierQueue, next, parents)
//  for v ∈ sharedFrontierQueue do
//      for u ∈ neighbors[v] do
//          if parents[u] = -1 then
//              parents[u] ← v
//              next ← next ∪ {u}
//          end if
//      end for
//  end for

int topDownStepGraphCSR(struct Graph* graph, struct ArrayQueue* sharedFrontierQueue, struct ArrayQueue** localFrontierQueues){


    
    int v;
    int u;
    int i;
    int j;
    int edge_idx;
    int mf = 0;


        //create a parallel region here
        // then use a local queue for each thread id
        struct ArrayQueue* localFrontierQueue = localFrontierQueues[0];
        

        //parallelize this loop using pargma for notice the conflicts and the enqueue bottleneck
        //in BFS you can process each frontier in parallel and generate a local next frontier
        //a good trick is to do a compare and swap operation and then use that to enqueue your element to the next frontier
        //or do an atomic enqueue either way should give you different performances. 
        for(i = sharedFrontierQueue->head ; i < sharedFrontierQueue->tail; i++){
            v = sharedFrontierQueue->queue[i];
            edge_idx = graph->vertices[v].edges_idx;
            for(j = edge_idx ; j < (edge_idx + graph->vertices[v].out_degree) ; j++){
                u = graph->sorted_edges_array[j].dest;
                int u_parent = graph->parents[u]; 
                if(u_parent < 0 ){
                    graph->parents[u] = v; // this operation need to be atomic 
                    enArrayQueue(localFrontierQueue, u); // instead of using local queues try using one shared atomic queue
                    mf++;
                }
            }

        } 


        // use the arrayQueueflush function to flush local frontiers into on shared frontier here
         //you need to make it work for parallel threads
        flushArrayQueueToShared(localFrontierQueue,sharedFrontierQueue);


        // end parallel region

       




    
    return mf;
}



// breadth-first-search(graph, source)
//  sharedFrontierQueue ← {source}
//  next ← {}
//  parents ← [-1,-1,. . . -1]
//      while sharedFrontierQueue 6= {} do
//          bottom-up-step(graph, sharedFrontierQueue, next, parents)
//          sharedFrontierQueue ← next
//          next ← {}
//      end while
//  return parents

void breadthFirstSearchGraphPull(int source, struct Graph* graph){

    
    struct Timer* timer = (struct Timer*) malloc(sizeof(struct Timer));
    struct Timer* timer_inner = (struct Timer*) malloc(sizeof(struct Timer));
    double inner_time = 0;
    struct Bitmap* bitmapCurr = newBitmap(graph->num_vertices);
    struct Bitmap* bitmapNext = newBitmap(graph->num_vertices);

    int nf = 0; // number of vertices in sharedFrontierQueue
   

    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Starting Breadth First Search (PULL)");
    printf(" -----------------------------------------------------\n");
    printf("| %-51d | \n", source);
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15s | %-15s | \n", "Iteration", "Nodes", "Time (Seconds)");
    printf(" -----------------------------------------------------\n");

    if(source < 0 && source > graph->num_vertices){
        printf(" -----------------------------------------------------\n");
        printf("| %-51s | \n", "ERROR!! CHECK SOURCE RANGE");
        printf(" -----------------------------------------------------\n");
        return;
    }

 
    Start(timer_inner);
    setBit(bitmapNext,source);
    bitmapNext->numSetBits = 1;
    graph->parents[source] = source;  
    swapBitmaps(&bitmapCurr, &bitmapNext);
    clearBitmap(bitmapNext);
    Stop(timer_inner);
    nf = 1;
    graph->processed_nodes += nf;
    inner_time +=  Seconds(timer_inner);
    printf("| BU %-12u | %-15u | %-15f | \n",graph->iteration++, nf , Seconds(timer_inner));

    Start(timer);
    while(bitmapCurr->numSetBits){ // start while 

                Start(timer_inner);
                    nf = bottomUpStepGraphCSR(graph,bitmapCurr,bitmapNext);
                    swapBitmaps(&bitmapCurr, &bitmapNext);
                    bitmapCurr->numSetBits = nf;
                    clearBitmap(bitmapNext);
                Stop(timer_inner);

                //stats collection
                inner_time +=  Seconds(timer_inner);
                graph->processed_nodes += nf;
                printf("| BU %-12u | %-15u | %-15f | \n",graph->iteration++, nf , Seconds(timer_inner));
            
    } // end while
    Stop(timer);
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15d | %-15f | \n","No OverHead", graph->processed_nodes, inner_time);
    printf(" -----------------------------------------------------\n");
    printf(" -----------------------------------------------------\n");
    printf("| %-15s | %-15d | %-15f | \n","total", graph->processed_nodes, Seconds(timer));
    printf(" -----------------------------------------------------\n");


    freeBitmap(bitmapNext);
    freeBitmap(bitmapCurr);
    free(timer);
    free(timer_inner);
}




// bottom-up-step(graph, sharedFrontierQueue, next, parents) //pull
//  for v ∈ vertices do
//      if parents[v] = -1 then
//          for u ∈ neighbors[v] do
//              if u ∈ sharedFrontierQueue then
//              parents[v] ← u
//              next ← next ∪ {v}
//              break
//              end if
//          end for
//      end if
//  end for

int bottomUpStepGraphCSR(struct Graph* graph, struct Bitmap* bitmapCurr, struct Bitmap* bitmapNext){


    int v;
    int u;
    int j;
    int edge_idx;
    int out_degree;
    
    int nf = 0; // number of vertices in next frontier
   

    // you need to insert a pragma here parallelize this loop
    // we are collecting statistics here especially nf variable means number of set bits in the next frontier use reduction for it
    // make sure you set shared and private variables correctly
    // The set bit needs to be atomic operation
    // since each v is updating its own parent you will notices no need for locks.

    for(v=0 ; v < graph->num_vertices ; v++){
                out_degree = graph->inverse_vertices[v].out_degree;
                if(graph->parents[v] < 0){ 
                    edge_idx = graph->inverse_vertices[v].edges_idx;

                    for(j = edge_idx ; j < (edge_idx + out_degree) ; j++){
                         u = graph->inverse_sorted_edges_array[j].dest;
                         if(getBit(bitmapCurr, u)){
                            graph->parents[v] = u;
                            setBit(bitmapNext, v);
                            nf++;
                            break;
                         }
                    }
                }
        
    }
    return nf;
}