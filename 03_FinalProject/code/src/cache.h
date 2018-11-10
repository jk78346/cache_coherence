/*******************************************************
                          cache.h
********************************************************/

#ifndef CACHE_H
#define CACHE_H

#include <cmath>
#include <iostream>

typedef unsigned long ulong;
typedef unsigned char uchar;
typedef unsigned int uint;

/****add new states, based on the protocol****/
enum{
	INVALID = 0,
	SHARED,
   EXCLUSIVE,
	MODIFIED
};
// define bus actions
enum{
   NONE = 0,
   BusRd,
   BusRdX,
   Flush,
   FlushOpt,
   BusUpgr
};

extern int COPIES_EXIST;
extern int protocol;
extern int c2c_FLAG;
extern int BusRdX_FlushOpt_FLAG; // active proc: I -> M, other(s): E(S) -> I, the mem_trans_cnt doesn't count


class cacheLine 
{
protected:
   ulong tag;
   ulong Flags;   // 0:invalid, 1:valid, 2:dirty 
   ulong seq; 
 
public:
   cacheLine()            { tag = 0; Flags = 0; }
   ulong getTag()         { return tag; }
   ulong getFlags()			{ return Flags;}
   ulong getSeq()         { return seq; }
   void setSeq(ulong Seq)			{ seq = Seq;}
   void setFlags(ulong flags)			{  Flags = flags;}
   void setTag(ulong a)   { tag = a; }
   void invalidate()      { tag = 0; Flags = INVALID; }//useful function
   bool isValid()         { return ((Flags) != INVALID); }
};

class Cache
{
protected:
   ulong size, lineSize, assoc, sets, log2Sets, log2Blk, tagMask, numLines;
   ulong reads,readMisses,writes,writeMisses,writeBacks;

   //******///
   //add coherence counters here///
   ulong c2c_cnt;    //cache-to-cache transfers
   ulong mem_trans_cnt;    //memory transactions
   ulong iterv_cnt;     //interventions (E/M to S state)
   ulong invalid_cnt;   //invalidations
   ulong flushes_cnt;
   ulong BusRdX_cnt;
   //******///
   int RM_FLAG; // designed for MESI, in read miss case

   cacheLine **cache;
   ulong calcTag(ulong addr)     { return (addr >> (log2Blk) );}
   ulong calcIndex(ulong addr)  { return ((addr >> log2Blk) & tagMask);}
   ulong calcAddr4Tag(ulong tag)   { return (tag << (log2Blk));}
   
public:
    ulong currentCycle;  
     
    Cache(int,int,int);
   ~Cache() { delete cache;}
   
   cacheLine *findLineToReplace(ulong addr);
   cacheLine *fillLine(ulong addr);
   cacheLine * findLine(ulong addr);
   cacheLine * getLRU(ulong);
   
   ulong getRM(){return readMisses;} ulong getWM(){return writeMisses;} 
   ulong getReads(){return reads;}ulong getWrites(){return writes;}
   ulong getWB(){return writeBacks;}
   
   void writeBack(ulong)   {writeBacks++;}
   ulong Access(ulong,uchar);
   void printStats(uint id);
   void updateLRU(cacheLine *);

   //******///
   //add other functions to handle bus transactions///
   //******///
   ulong snoopBus(ulong, ulong);
   void snoopReaction(ulong);
   ulong MSI_snoop_handle(ulong, cacheLine * line);
   ulong MESI_snoop_handle(ulong, cacheLine * line);
   ulong Dragon_snoop_handle(ulong, cacheLine * line);
   void proc_handle(ulong);
   // add detail functions for both access() and snoopBUs() to deal with three protocols
   ulong readHit(cacheLine *);
   ulong writeHit(cacheLine *);
   ulong readMiss(cacheLine *);
   ulong writeMiss(cacheLine *); 
};

#endif
