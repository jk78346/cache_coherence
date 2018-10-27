# Machine Problem 2

## Tasks:
### In this machine problem you will
* Be provided with two serial implementations of BFS Pull (bottom-up-step)/Push (top-down-step), Queue, Bitmap data structures.
* You will parallelize the BFS algorithms using OpenMP.
* This parallelization will require you to change how you update the Queue and Bitmap.
* You need to solve the race conditions when transitioning from serial to parallel implementation,for BFS.
* Queues need to be multithread friendly.
* Bitmap need to be multithread friendly
* You need to provide atomic implementations for BFS, Queue and Bitmap using
    * OpenMP atomics.
    * GCC atomic built-ins.
    * OpenMP locks.
    * OpenMP critical sections.
* Report the difference in performance using these four approaches.
* Mention in you report the difference and elaborate why.
* Explain why you needed to use/not use atomics with both BFS implementations (Push/Pull).
* **For extra credit (5 points) look at the assembly code and show what instruction are generated for each approach, this will help you to reason why your performance is same/better/degraded.**

## Motivation
In the previous machine problem, we showed how to build an optimized memory friendly graph to be used for fast graph processing algorithms. In this machine problem we will be utilizing the adjacency list structure we built for Breadth Search First (BFS). An algorithm for searching or traversing tree or graph data structure, using two approaches. One that uses Push data flow where nodes in the frontier (will be explained later) update neighboring nodes, another is Pull where nodes update themselves and pull information from neighboring nodes in the frontier. Each approach has its own benefits. Understanding BFS is not only useful for passing this assignment. It is considered a core behavioral pattern for many other graph algorithms and applications. So, optimizing and understanding it might come in handy in the future. Here are some of the places you might find BFS.
* Shortest Path and Minimum Spanning Tree for unweighted graph.
* Peer to Peer Networks.
* Crawlers in Search Engines.
* GPS Navigation systems.
* Social Networking Websites.
* Broadcasting in Network.
* In Garbage Collection.
* Cycle detection in undirected graph.
* Ford–Fulkerson algorithm.
* To test if a graph is Bipartite.
* Path Finding.
* Finding all nodes within one connected component.
* Many algorithms like Prim’s Minimum Spanning Tree and Dijkstra’s Single Source Shortest Path
* use structure like Breadth First Search.

## Please note the following

* **It is recommended to always to go back to the book as a reference.**
    * (NEW) Fundamentals of Parallel Multi-core Architecture (Chapman & Hall/CRC Computational Science) 1st Edition-2015 by Yan Solihin (Author).
    * Fundamentals of Parallel Computer Architecture Paperback-2009 by Yan Solihin (Author).
* For now this repository is for FALL 2018 Class any future updates are the responsibility of future TAs if they want to use it.
* This framework developed on Ubuntu-Linux. It is your responsibility to make it work on windows or other systems, though it will run on any Linux machine that has a terminal.

## Organization
* `02_MP2` - Machine Problem 2
  * `code` - base code for the problem
    * `bin`- binaries are generated in this folder
    * `datasets` - sample data sets for the code
    * `include` - [.h] header files to be included in the code
        * `bfs.h` - push/pull breadth first search implementation to be prallelized 
        * `graph.h` - graph abstraction contains edgelists and inverse edgelists with their mappings 
        * `bitmap.h` - bitmap implementaion not multithread safe
        * `arrayQueue.h` - simple array based queue not multithread safe  
        * `edgelist.h`
        * `sort.h`
        * `timer.h`
        * `vertex.h`
    * `obj` - [.o] object files generated here for linking
    * `src` - source code resides in this directory
        * `bfs.c` - push/pull breadth first search implementation to be prallelized 
        * `graph.c` - graph abstraction contains edgelists and inverse edgelists with their mappings
        * `bitmap.c` - bitmap implementaion not multithread safe
        * `arrayQueue.c` - simple array based queue not multithread safe 
        * `edgelist.c`
        * `sort.c` 
        * `timer.c`
        * `vertex.c`
        * `main.c` - **your parallel/serial BFS called here**
    * `Makefile` - a file that compiles the base code
  * `doc` - Documents describes the Machine Problem

## Details
### Benchmark format
* Edge lists are provided in textual format and they are formatted as source → destination separated by a tab.
*Src*  →  *Dest*

### Initial setup
Unlike MP1 you can change the thread number and file name dynamically from the terminal also the source node for the BFS algorithm
#### Usage from binary file:
```bash
  Usage: ./main -f <graph file> -r [root] -n [num threads] -p [0-push/1-pull]
  -f <graph file.txt>
  -h [help]
  -r [root/source]: BFS 
  -p [algorithm direction] [0-1]-push/pull
  -n [num threads] default:max number of threads the system has
  ```
#### Usage from Makefile:
```bash
  make run f=<graph file> r=[root] n=[num threads] p=[0-push/1-pull]
  f <graph file.txt>
  r [root/source]: BFS 
  p [algorithm direction] [0-1]-push/pull
  n [num threads] default:max number of threads the system has
  ```
 #### Steps:
1. After cloning the repository. enter the directory
  ```bash
  cd code
  ```
2. compile the code
  ```bash
  make
  ```
3. run small test file
  ```bash
  make run
  ```
4. run the code
  ```bash
  make run
  ```