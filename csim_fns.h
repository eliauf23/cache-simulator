

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#define MIN_BLOCK_SIZE 4U
#define ADDRESS_LEN 32U


#include "csim_fns.h"
#include <iostream>
#include <string>
#include <bitset>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <cstdio>

namespace CacheSimulator
{

    std::string getIndex(int idxLen, int tagLen, unsigned int address);

    std::string getTag(int tagLen, unsigned int address);

    uint32_t getValidInteger(std::string str);

    //returns -1 if cache miss, else returns index in cache
    unsigned int isPowerOfTwo(uint32_t num);

    int printErrorMsg(const std::string &errorMsg);
    //input parsing functions

    int checkIfArgsValid(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5,
                         std::string s6);
}

#endif //CSIM_FNS_H