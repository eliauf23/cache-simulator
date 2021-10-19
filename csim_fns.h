

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

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
#define MIN_BLOCK_SIZE 4U

#include "csim_fns.h"
#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>






//input parameters are num sets = s, num blocks = b, num bytes per block= block_size
//

namespace CacheSimulator {

uint32_t getValidInteger(std::string str);

//prints out results with required formatting
void displayResults(uint32_t loads, uint32_t stores, uint32_t loadHits, uint32_t loadMisses, uint32_t storeHits, uint32_t storeMisses, uint32_t cycles);

//returns 1 if number is a power of 2, 0 if not a power of 2
unsigned int isPowerOfTwo(uint32_t num);

//returns -1 if cache miss, else returns index in cache
//int findAddressInCache(Cache cache, uint32_t address);

unsigned int isPowerOfTwo(uint32_t num);

int printErrorMsg(const std::string& errorMsg);
    //input parsing functions

void displayResults(int loadHits, int loadMisses, int storeHits, int storeMisses, int totalCycles);

}

#endif  //!__CSIM_FNS__H__