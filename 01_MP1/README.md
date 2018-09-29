# Machine Problem 1

## Motivation

Graph processing is widely used to solve big data problems in multiple domains, such as social networks, protein interactions in bioinformatics and web graph hierarchies. The cost of pre-processing (building the graph data structure) in many cases dominates the algorithm execution step. This calls into question the benefit of proposed algorithmic optimization that relies on extensive preprocessing. For example, fast breadth-first search (BFS) algorithms, rely on Compressed Sparse Row (CSR) representation of the graph. The processes of reading the graph list into a CSR representation and forming the adjacency list usually takes up to 90% of the execution time!.

**In this Project you will work on two versions of this preprocessing stage in building Compressed Sparse Row (CSR), using Radix sort, a serial version, an OpenMP version. Provided with a framework that sorts and builds a CSR with a baseline model using count sort.**

## Please note the following

* **It is recommended to always to go back to the book as a reference.**
    * (NEW) Fundamentals of Parallel Multi-core Architecture (Chapman & Hall/CRC Computational Science) 1st Edition-2015 by Yan Solihin (Author).
    * Fundamentals of Parallel Computer Architecture Paperback-2009 by Yan Solihin (Author).
* For now this repository is for FALL 2018 Class any future updates are the responsibility of future TAs if they want to use it.
* This framework developed on Ubuntu-Linux. It is your responsibility to make it work on windows or other systems, though it will run on any Linux machine that has a terminal.

## Organization
* `01_MP1` - Machine Problem 1
  * `code` - base code for the problem
    * `bin`- binaries are generated in this folder
    * `datasets` - sample data sets for the code
    * `include` - [.h] header files to be included in the code
        * `bfs.h` - simple breadth first search implementation 
        * `edgelist.h`
        * `sort.h` - **parallel/serial radix sort function header here**
        * `timer.h`
        * `vertex.h`
    * `obj` - [.o] object files generated here for linking
    * `src` - source code resides in this directory
        * `bfs.c` - simple breadth first search implementation 
        * `edgelist.c`
        * `sort.c` - **parallel/serial radix sort body here**
        * `timer.c`
        * `vertex.c`
        * `main.c` - **your parallel/serial radix sort called here**
    * `Makefile` - a file that compiles the base code
  * `doc` - Documents describes the Machine Problem

## Details
### Benchmark format
* Edge lists are provided in textual format and they are formatted as source → destination separated by a tab.
*Src*  →  *Dest*

### Initial setup
1. After cloning the repository. enter the directory
  ```bash
  cd code
  ```
2. compile the code
  ```bash
  make
  ```
3. run the code
  ```bash
  make run
  ```