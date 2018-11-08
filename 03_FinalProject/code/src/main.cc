/*******************************************************
                          main.cc
********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <fstream>
#include <vector> 
using namespace std;

#include "cache.h"

unsigned long char2long(char *addr){

	return 0;
}

int main(int argc, char *argv[])
{
	
	ifstream fin;
	FILE * pFile;
	char *line = NULL;
	size_t len = 0;
	const char *delimiter = " ";
	int proc_id;
	unsigned char op;
	unsigned long addr;

	unsigned long busAction; // a global behavior

	if(argv[1] == NULL){
		 printf("input format: ");
		 printf("./smp_cache <cache_size> <assoc> <block_size> <num_processors> <protocol> <trace_file> \n");
		 exit(0);
        }

	/*****uncomment the next five lines*****/
	int cache_size = atoi(argv[1]);
	int cache_assoc= atoi(argv[2]);
	int blk_size   = atoi(argv[3]);
	int num_processors = atoi(argv[4]);/*1, 2, 4, 8*/
	int protocol   = atoi(argv[5]);	 /*0:MSI, 1:MESI, 2:Dragon*/
	char *fname =  (char *)malloc(20);
 	fname = argv[6];
	
	//****************************************************//
	//**printf("===== Simulator configuration =====\n");**//
	//*******print out simulator configuration here*******//
	//****************************************************//
	printf("===== 506 Personal information =====\n");
	printf("Kuan-Chieh Hsu\n");
	printf("200259954\n");
	printf("ECE492 Students? NO\n");
	printf("===== 506 SMP Simulator configuration =====\n");
	printf("L1_SIZE: %d\n", cache_size);
	printf("L1_ASSOC: %d\n", cache_assoc);
	printf("L1_BLOCKSIZE: %d\n", blk_size);
	printf("NUMBER OF PROCESSORS: %d\n", num_processors);
	printf("COHERENCE PROTOCOL: %s\n", (protocol == 0)?"MSI":((protocol == 1)?"MESI":((protocol ==2)?"Dragon":"--")));
	printf("TRACE FILE: %.27s\n", &fname[3]);
 
	//*********************************************//
        //*****create an array of caches here**********//
	//*********************************************//	
 	Cache *myCaches[num_processors];
 	for(int i = 0 ; i < num_processors ; i++){
 		myCaches[i] = new Cache(cache_size, cache_assoc, blk_size);
 	}

	pFile = fopen (fname,"r");
	if(pFile == 0)
	{   
		printf("Trace file problem\n");
		exit(0);
	}
	///******************************************************************//
	//**read trace file,line by line,each(processor#,operation,address)**//
	//*****propagate each request down through memory hierarchy**********//
	//*****by calling cachesArray[processor#]->Access(...)***************//
	///******************************************************************//

	while((getline(&line, &len, pFile)) != -1){ //iterate line by line
		proc_id = atoi(strtok(line, delimiter));
		op      = strtok(NULL, delimiter)[0];	
		sscanf(((string)(strtok(NULL, delimiter))).c_str(), "%lx", &addr);
		// printf("proc_id: %d, op: %c, addr: %lx\n", proc_id, op, addr);
		busAction = myCaches[proc_id]->Access(addr, op, protocol);
		// printf("proc_id: %d, addr: %lx, op: %c, busAction: %lu\n", proc_id, addr, op, busAction);
		// for this moment, all other caches can snoop the bus to react 
		// since no state transition if no access from any proc.
		if(busAction != NONE){ // other processors have to snoop bus and react 
			for(int i = 0 ; i < num_processors ; i++){
				if(i != proc_id){
					myCaches[i]->snoopBus(busAction, addr, protocol);
				}
			}
		}
	}

	fclose(pFile);
	if(line){
		free(line);
	}
	//********************************//
	//print out all caches' statistics //
	//********************************//
	for(int i = 0 ; i < num_processors ; i++){
		myCaches[i]->printStats(i);
	}
}
