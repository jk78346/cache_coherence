/*******************************************************
                          cache.cc
********************************************************/

#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include "cache.h"
using namespace std;

Cache::Cache(int s,int a,int b )
{
   ulong i, j;
   reads = readMisses = writes = 0; 
   writeMisses = writeBacks = currentCycle = 0;

   size       = (ulong)(s);
   lineSize   = (ulong)(b);
   assoc      = (ulong)(a);   
   sets       = (ulong)((s/b)/a);
   numLines   = (ulong)(s/b);
   log2Sets   = (ulong)(log2(sets));   
   log2Blk    = (ulong)(log2(b));   
  
   //*******************//
   //initialize your counters here//
   //*******************//
   c2c_cnt = mem_trans_cnt = iterv_cnt = 0;
   invalid_cnt = flushes_cnt = BusRdX_cnt = 0;


   tagMask =0;
   for(i=0;i<log2Sets;i++)
   {
		tagMask <<= 1;
        tagMask |= 1;
   }
   
   /**create a two dimentional cache, sized as cache[sets][assoc]**/ 
   cache = new cacheLine*[sets];
   for(i=0; i<sets; i++)
   {
      cache[i] = new cacheLine[assoc];
      for(j=0; j<assoc; j++) 
      {
	   cache[i][j].invalidate();
      }
   }      
   
}

/**you might add other parameters to Access()
since this function is an entry point 
to the memory hierarchy (i.e. caches)**/
ulong Cache::Access(ulong addr,uchar op, int protocol)
{
	ulong busAction;
	currentCycle++;/*per cache global counter to maintain LRU order among cache ways, updated on every cache access*/
    if(op == 'w') writes++;
	else          reads++;
	
	cacheLine * line = findLine(addr);
	if(line == NULL){/*miss*/
		cacheLine *newline = fillLine(addr);
   		if(op == 'w'){  
   			busAction = writeMiss(newline, protocol); 
		}else{ // op == 'r' 
			busAction = readMiss(newline, protocol);
		}
	}else{ /**since it's a hit, update LRU and update dirty flag**/
		updateLRU(line);
		if(op == 'w'){
			busAction = writeHit(line, protocol);
		}else{
			busAction = readHit(line, protocol);
		}
	}
	return busAction;
}

/*look up line*/
cacheLine * Cache::findLine(ulong addr)
{
   ulong i, j, tag, pos;
   
   pos = assoc;
   tag = calcTag(addr);
   i   = calcIndex(addr);
  
   for(j=0; j<assoc; j++)
	if(cache[i][j].isValid())
	        if(cache[i][j].getTag() == tag)
		{
		     pos = j; break; 
		}
   if(pos == assoc)
	return NULL;
   else
	return &(cache[i][pos]); 
}

/*upgrade LRU line to be MRU line*/
void Cache::updateLRU(cacheLine *line)
{
  line->setSeq(currentCycle);  
}

/*return an invalid line as LRU, if any, otherwise return LRU line*/
cacheLine * Cache::getLRU(ulong addr)
{
   ulong i, j, victim, min;

   victim = assoc;
   min    = currentCycle;
   i      = calcIndex(addr);
   
   for(j=0;j<assoc;j++)
   {
      if(cache[i][j].isValid() == 0) return &(cache[i][j]);     
   }   
   for(j=0;j<assoc;j++)
   {
	 if(cache[i][j].getSeq() <= min) { victim = j; min = cache[i][j].getSeq();}
   } 
   assert(victim != assoc);
   
   return &(cache[i][victim]);
}

/*find a victim, move it to MRU position*/
cacheLine *Cache::findLineToReplace(ulong addr)
{
   cacheLine * victim = getLRU(addr);
   updateLRU(victim);
  
   return (victim);
}

/*allocate a new line*/
cacheLine *Cache::fillLine(ulong addr)
{ 
	ulong tag;
  
	cacheLine *victim = findLineToReplace(addr);
	assert(victim != 0);
	if(victim->getFlags() == MODIFIED){
   		writeBack(addr);
   		mem_trans_cnt++;
	}
	tag = calcTag(addr);   
	victim->setTag(tag);
   // victim->setFlags(VALID);    // do this outside of this function
   /**note that this cache line has been already 
      upgraded to MRU in the previous function (findLineToReplace)**/

	return victim;
}

void Cache::printStats(uint id)
{ 
	printf("============ Simulation results (Cache %d) ============\n", id);
	/****print out the rest of statistics here.****/
	/****follow the ouput file format**************/
	printf("01. number of reads:				%lu\n", reads);
	printf("02. number of read misses:			%lu\n", readMisses);
	printf("03. number of writes:				%lu\n", writes);
	printf("04. number of write misses:			%lu\n", writeMisses);
	printf("05. total miss rate:				%4.2f%%\n", ((double)(readMisses + writeMisses)/(double)(reads + writes))*100 );
	printf("06. number of writebacks:			%lu\n", writeBacks);
	printf("07. number of cache-to-cache transfers:		%lu\n", c2c_cnt);
	printf("08. number of memory transactions:		%lu\n", mem_trans_cnt);
	printf("09. number of interventions:			%lu\n", iterv_cnt);
	printf("10. number of invalidations:			%lu\n", invalid_cnt);
	printf("11. number of flushes:				%lu\n", flushes_cnt);
	printf("12. number of BusRdX:				%lu\n", BusRdX_cnt);
}

int Cache::snoopBus(ulong busAction, ulong addr, int protocol){
	cacheLine * line = findLine(addr);
	int result = 0;
	if(protocol == 0){ // MSI
		MSI_snoop_handle(busAction, line);
	}else if(protocol == 1){ // MESI
		MULTI_SHARE_FLAG = 0; // init
		ulong tmp_c2c_cnt = c2c_cnt;
		result = MESI_snoop_handle(busAction, line);
		if(MULTI_SHARE_FLAG == 1 && c2c_cnt > tmp_c2c_cnt + 1){ //for multiple share case, the counter only count once
			printf("tmp_c2c_cnt = %lu, c2c_cnt = %lu\n", tmp_c2c_cnt, c2c_cnt);
			c2c_cnt = tmp_c2c_cnt + 1;
		}
	}else if(protocol == 2){ // Dragon
		Dragon_snoop_handle(busAction, line);
	}else{
		printf("undefined protocol: %d\n", protocol);
		exit(0);
	}
	return result; // designed only for checking COPIES_EXIST, otherwise should return 0
}

void Cache::COPIES_EXIST_handle(ulong addr){
	cacheLine * line = findLine(addr);
	if(line != NULL){ //assert that the cache line is there and wait for state determination
		line->setFlags(SHARED);
	}else{
		printf("COPIES_EXIST_handling but cache line not valid.");
	}
}

ulong Cache::writeMiss(cacheLine * newline, int protocol){
	ulong busAction;
	if(protocol == 0){ // MSI
		// I -> M
		writeMisses++;
		mem_trans_cnt++;
		busAction = BusRdX;
		BusRdX_cnt++;
		newline->setFlags(MODIFIED); 
	}else if(protocol == 1){ // MESI
		// I -> M
		writeMisses++;
		mem_trans_cnt++;
		busAction = BusRdX;
		BusRdX_cnt++;
		newline->setFlags(MODIFIED);
	}else if(protocol == 2){ // Dragon

	}else{
		printf("undefined protocol: %d\n", protocol);
		exit(0);
	}
	return busAction;
}

ulong Cache::readMiss(cacheLine * newline, int protocol){
	ulong busAction;
 	if(protocol == 0){ // MSI
		// I -> S
		readMisses++;
		busAction = BusRd;
		mem_trans_cnt++;
		newline->setFlags(SHARED);
 	}else if(protocol == 1){ // MESI
		// could be either I -> E or I -> S depend on COPIES_EXIST line
		// and only read miss can will put BusRd on Bus.
 		readMisses++;
		busAction = BusRd;
		mem_trans_cnt++;
		newline->setFlags(EXCLUSIVE); //its temperary, futher could be S if COPIES_EXIST == 1 else remains E 
		// leave newline->setFlags() for futher determine. (after snooping done by all others to decide COPIES_EXIST)
	}else if(protocol == 2){ // Dragon

	}else{
		printf("undefined protocol: %d\n", protocol);
		exit(0);
	}
 	return busAction;
}

ulong Cache::writeHit(cacheLine * line, int protocol){
	ulong busAction;
	if(protocol == 0){ // MSI
		if(line->getFlags() == SHARED){
	 		line->setFlags(MODIFIED);
			mem_trans_cnt++; // S -> M : memory controller provide data anyway since it cannot tell if (1) S -> M or (2) I -> M
			busAction = BusRdX;
	 		BusRdX_cnt++;	// in this case, it generate BusRdX, no distinguish between I -> M's BusRdX
		}else{
			busAction = NONE;
		}
	}else if(protocol == 1){ // MESI
		if(line->getFlags() == SHARED){ // S -> M
			line->setFlags(MODIFIED);
			busAction = BusUpgr; // this $ simply S -> M, and notifiy others to do: S -> I, no memory access
		}else if(line->getFlags() == EXCLUSIVE){ // E -> M
			line->setFlags(MODIFIED); // simply E -> M and no busAction since I know no one shares the same line
		}else{
			busAction = NONE; // still, it remains in M
		}
	}else if(protocol == 2){ // Dragon

	}else{
		printf("undefined protocol: %d\n", protocol);
		exit(0);
	}
	return busAction;
}

ulong Cache:: readHit(cacheLine * line, int protocol){
	ulong busAction;
	if(protocol == 0){ // MSI
		busAction = NONE;
	}else if(protocol == 1){ // MESI
		busAction = NONE; 
	}else if(protocol == 2){ // Dragon

	}else{
		printf("undefined protocol: %d\n", protocol);
		exit(0);
	}
	return busAction;
}

void Cache::MSI_snoop_handle(ulong busAction, cacheLine * line){
	ulong status;
	if(line != NULL){ // snoop bus action on addr that I have valid copy in my cache
		status = line->getFlags();
		if(status == MODIFIED){
			if(busAction == BusRd){ // M -> S
				line->setFlags(SHARED);
				writeBacks++;
				iterv_cnt++;
			}else if(busAction == BusRdX){ // M-> I
				line->invalidate();
				writeBacks++;	//dirty has to be write back for consistency when invalidate
				invalid_cnt++;
			}else{
				printf("undefined bus action: %lu\n", busAction);
				exit(0);
			}
			// ideally do flush from cache to memory
			flushes_cnt++;
			mem_trans_cnt++; // either M -> S or M -> I require data write back to memory
		}else if(status == SHARED){
			if(busAction == BusRdX){ // S-> I
				line->invalidate();
				invalid_cnt++;
			}
		}else if(status == INVALID){
			return;
		}else{
			printf("undefined state: %lu in MSI\n", busAction);
			exit(0);
		}
	}
}

int Cache::MESI_snoop_handle(ulong busAction, cacheLine * line){
	ulong status;
	int COPIES_EXIST = 0;
	if(line != NULL){// snoop bus action on addr that I have valid copy in my cache
		status = line->getFlags();
		if(status == MODIFIED){
			if(busAction == BusRd){ // Flush, M -> S
				line->setFlags(SHARED);
				writeBacks++;
				mem_trans_cnt++; // Flush to bus, both memory and other shares have to pick up
				// c2c_cnt++;
				iterv_cnt++;
				flushes_cnt++;
				COPIES_EXIST = 1;
			}
			if(busAction == BusRdX){ // Flush, M -> I 
				line->setFlags(INVALID);
				writeBacks++;
				mem_trans_cnt++;
				flushes_cnt++;
				invalid_cnt++;
			}
		}else if(status == EXCLUSIVE){
			if(busAction == BusRd){ // FlushOpt, E -> S
				line->setFlags(SHARED);
				c2c_cnt++;
				//flushes_cnt++;
				iterv_cnt++;
				COPIES_EXIST = 1;
			}
			if(busAction == BusRdX){ // FlushOpt, E -> I
				line->setFlags(INVALID);
				c2c_cnt++;            
				//flushes_cnt++;
				invalid_cnt++;
			}
		}else if(status == SHARED){
			if(busAction == BusRd){	//FlushOpt, S -> S (multiple shared copies will all flush, since no owner concept here)
				// c2c_cnt++;
				MULTI_SHARE_FLAG = 1;
				//flushes_cnt++;	
				COPIES_EXIST = 1; // only from ( read miss on I state ) of active processor, ack this info to it
			}
			if(busAction == BusRdX){ // FlushOpt, S -> I
				line->setFlags(INVALID);
				// c2c_cnt++;
				MULTI_SHARE_FLAG = 1;
				//flushes_cnt++;
				invalid_cnt++;
			}
			if(busAction == BusUpgr){ // S -> I
				line->setFlags(INVALID);
				invalid_cnt++;
			}
		}else if(status == INVALID){
			;
		}else{
			printf("undefined state: %lu in MESI\n", busAction);
			exit(0);
		}
	}
	return COPIES_EXIST;
}

void Cache::Dragon_snoop_handle(ulong busAction, cacheLine * line){

}
