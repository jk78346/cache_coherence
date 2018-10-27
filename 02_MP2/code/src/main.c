#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <omp.h>
#include <memory.h>

#include "graph.h"
#include "bfs.h"
#include "sort.h"
#include "edgelist.h" 
#include "vertex.h"
#include "timer.h"

int numThreads;

void printMessageWithtime(const char * msg, double time){

    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", msg);
    printf(" -----------------------------------------------------\n");
    printf("| %-51f | \n", time);
    printf(" -----------------------------------------------------\n");

}

static void usage(void) {
  printf("\nUsage: ./main -f <graph file> -r [root] -n [num threads] -p [0-push/1-pull]\n");
  printf("\t-f <graph file.txt>\n");
  printf("\t-h [help]\n");
  printf("\t-r [root/source]: BFS \n");
  printf("\t-p [algorithm direction] [0-1]-push/pull\n");
  printf("\t-n [num threads] default:max number of threads the system has\n");
  // _exit(-1);
}


int main(int argc, char **argv) {

 
  char *fvalue = NULL;
  char *rvalue = NULL;
  char *nvalue = NULL;
  char *pvalue = NULL;

  int root = 0;
  int pushpull = 0;


  numThreads = omp_get_max_threads();
  char *fnameb = NULL;

  int c;
  opterr = 0;

  while ((c = getopt (argc, argv, "f:p:r:n:h")) != -1){
    switch (c)
      {
      case 'h':
        usage();
        break;
      case 'f':
        fvalue = optarg;
        fnameb = fvalue;
        break;
      case 'r':
        rvalue = optarg;
        root = atoi(rvalue);
        break;
       break;
      case 'n':
        nvalue = optarg;
        numThreads = atoi(nvalue);
        break;
      case 'p':
        pvalue = optarg;
        pushpull = atof(pvalue);
        break;
      case '?':
        if (optopt == 'f')
          fprintf (stderr, "Option -%c <graph file> requires an argument  .\n", optopt);
        else if (optopt == 'r')
          fprintf (stderr, "Option -%c [root] requires an argument.\n", optopt);
        else if (optopt == 'n')
          fprintf (stderr, "Option -%c [num threads] requires an argument.\n", optopt);
        else if (optopt == 'p')
          fprintf (stderr, "Option -%c [push/pull] requires an argument.\n", optopt);
        else if (isprint (optopt))
          fprintf (stderr, "Unknown option `-%c'.\n", optopt);
        else
          fprintf (stderr,
                   "Unknown option character `\\x%x'.\n",
                   optopt);
        usage();
        return 1;
      default:
        abort ();
      }
    }
    

    //Set number of threads for the program
    omp_set_nested(1);
    omp_set_num_threads(numThreads);


    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "File Name");
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", fnameb);
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Number of Threads");
    printf(" -----------------------------------------------------\n");
    printf("| %-51u | \n", numThreads);
    printf(" -----------------------------------------------------\n");
  

    struct Timer* timer = (struct Timer*) malloc(sizeof(struct Timer));

    
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "New graph calculating size");
    printf(" -----------------------------------------------------\n");
    Start(timer);
    struct Graph* graph = newGraph(fnameb);
    Stop(timer);
    printMessageWithtime("New Graph Created",Seconds(timer));


    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Populate Graph with edges");
    printf(" -----------------------------------------------------\n");
    Start(timer);
    // populate the edge array from file
    loadEdgeArray(fnameb, graph);
    Stop(timer);
    printMessageWithtime("Time load edges to graph (Seconds)",Seconds(timer));


    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Radix Sort Graph");
    printf(" -----------------------------------------------------\n");
    Start(timer);
    graph = radixSortEdgesBySource(graph);
    Stop(timer);
    printMessageWithtime("Time Sorting (Seconds)",Seconds(timer));
    
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "Map vertices to Edges");
    printf(" -----------------------------------------------------\n");
    Start(timer);
    mapVertices(graph);
    Stop(timer);
    printMessageWithtime("Time Mapping (Seconds)",Seconds(timer));

    printf(" *****************************************************\n");
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "BFS Algorithm (PUSH/PULL)");
    printf(" -----------------------------------------------------\n");
    if(pushpull == 0){
        printf("| %-51s | \n", "PUSH");
    }
    else if (pushpull == 1){
        printf("| %-51s | \n", "PULL");
    }
    else
        printf("| %-51s | \n", "DEFAULT:PUSH");
    printf(" -----------------------------------------------------\n");
    printf("| %-51s | \n", "ROOT/SOURCE");
    printf(" -----------------------------------------------------\n");
    printf("| %-51u | \n", root);
    printf(" -----------------------------------------------------\n");
    Start(timer);
    
    if(pushpull == 0){
        breadthFirstSearchGraphPush(root, graph); // you need to parallelize this function
    }
    else if (pushpull == 1){
        breadthFirstSearchGraphPull(root, graph); // you need to parallelize this function
    }
    else
        breadthFirstSearchGraphPush(root, graph);

    Stop(timer);
    printMessageWithtime("Time BFS (Seconds)",Seconds(timer));

    Start(timer);
    freeGraph(graph);
    Stop(timer);
    printMessageWithtime("Free Graph (Seconds)",Seconds(timer));
    
    return 0;
}


