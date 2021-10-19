

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

namespace CacheSimulator {

    uint32_t getValidInteger(std::string str);

//returns -1 if cache miss, else returns index in cache
    unsigned int isPowerOfTwo(uint32_t num);

    int printErrorMsg(const std::string &errorMsg);
    //input parsing functions


    int checkIfArgsValid(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5,
                         std::string s6);
}


#endif  //CSIM_FNS_H