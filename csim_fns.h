#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>


//define constants:

#define MIN_BLOCK_SIZE 4U
/* assumptions from assignment description:
 * this is number of cycles to perform
 * load/store operation from specified location
 * to CPU for a single 4 byte quantity
 */
#define CACHE_LOAD_CYCLES 1U
#define CACHE_STORE_CYCLES 1U
#define MEM_LOAD_CYCLES 100U
#define MEM_STORE_CYCLES 100U
#define ADDRESS_LEN 32U


//use unsigned 32 bit integers for any memory addresses/indices
typedef struct {
    uint32_t dirty; //dirty bit - for write-back cache (tells you that you need to write changes back to memory before evicting block)
    uint32_t valid; //valid bit
    uint32_t tag;//tag - tells you the block you're in
    uint32_t evictionPriority; //higher value indicates that it's not being accessed
    // for LRU: measure of times set has been accessed, but this block hasn't been
} Block;

typedef struct {
    uint32_t numBlocks; //number of blocks in this set
    Block * blocks; //array of blocks
    uint32_t numUnusedBlocks;

} Set;

typedef struct {
    uint32_t numSets; //number of sets in this cache
    Set * sets; //array of sets
    uint32_t indexLen; //i.e. num bits in index
    uint32_t offsetLen; //num bits in offset
    //uint32_t addressLen = ADDRESS_LEN;
    //not needed in direct mapping, but might need number of blocks per set for associative mapping
} Cache;

int printErrorMsg(char * msg);

//prints out results with required formatting
void displayResults(uint32_t loads, uint32_t stores, uint32_t loadHits, uint32_t loadMisses, uint32_t storeHits, uint32_t storeMisses, uint32_t cycles);

//returns 1 if number is a power of 2, 0 if not a power of 2
unsigned int isPowerOfTwo(uint32_t num);

//returns -1 if cache miss, else returns index in cache
int findAddressInCache(Cache cache, uint32_t address);

unsigned int log_base2(unsigned int num);


#endif


