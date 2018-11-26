# Architecture Of Parallel Computers ECE 506 Projects and Material

This repository holds all machine problems, projects, and supplementary material for the ECE 506 course.

## Course Description

The need for parallel and massively parallel computers. Taxonomy of parallel computer architecture, and programming models for parallel architectures. Example parallel algorithms. Shared-memory vs. distributed-memory architectures. Correctness and performance issues. Cache coherence and memory consistency. Bus-based and scalable directory-based multiprocessors. Interconnection-network topologies and switch design. Brief overview of advanced topics such as multiprocessor pre-fetching and speculative parallel execution. Credit is not allowed for more than one course in this set: ECE 492, ECE 506.

## Please note the following

* **It is recommended to always to go back to the book as a reference.**
    * (NEW) Fundamentals of Parallel Multi-core Architecture (Chapman & Hall/CRC Computational Science) 1st Edition-2015 by Yan Solihin (Author).
    * Fundamentals of Parallel Computer Architecture Paperback-2009 by Yan Solihin (Author).
* For now this repository is for FALL 2018 Class any future updates are the responsibility of future TAs if they want to use it.
* This framework developed on Ubuntu-Linux. It is your responsibility to make it work on windows or other systems, though it will run on any Linux machine that has a terminal.

## Organization
* `00_Architecture_of_Parallel_Computers`
  * `00_ARC_HPC_Guide` - How to setup ARC/HPC NCSU for future Projects/MachineProblems(MPs).
  * `01_OpenMP` - OpenMP tutorials and guides with examples within the slides.
  * `02_Slides` - Slides from chapter 1-12 from the old book 2009.
  * `03_papers` - Extra reading material and papers (optional).
  * `04_Fall_2018` - Updated slides and notes for the semester the class being taught.
* `01_MP1` - Machine Problem 1
  * `code` - base code for the problem
    * `bin`- binaries are generated in this folder
    * `datasets` - sample data sets for the code
    * `include` - [.h] header files to be included in the code
    * `obj` - [.o] object files generated here for linking
    * `src` - source code resides in this directory
    * `Makefile` - a file that compiles the base code
  * `doc` - Documents describes the Machine Problem
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
* `03_FinalProject` - Machine Problem 3
  * `code` - base code for the problem
    * `trace`- trace data set
    * `val.v2` - validation data sets for the code
    * `src` - source code resides in this directory
        * `cache.cc`
        * `cache.h.cc`
        * `main.cc`
        * `Makefile` 

## Details
This will be added later.
### Initial setup
1. Clone the repository. enter the directory and initialize the submodules
  ```bash
  git clone https://github.ncsu.edu/atmughra/ECE-506-492-Projects.git
  cd ECE-506-492-Projects
  ```
