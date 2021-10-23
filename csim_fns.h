

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#define MIN_BLOCK_SIZE 4U


#include <iostream>
#include <string>

namespace CacheSimulator
{

    uint32_t getValidInteger(std::string str);

    //returns -1 if cache miss, else returns index in cache
    unsigned int isPowerOfTwo(uint32_t num);

    int printErrorMsg(const std::string &errorMsg);
    //input parsing functions

    int checkIfArgsValid(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5,
                         std::string s6);
}

#endif //CSIM_FNS_H