

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#define MIN_BLOCK_SIZE 4U


#include <iostream>
#include <string>
#include <cstring>


    int getValidInteger(std::string str);

    //returns -1 if cache miss, else returns index in cache
    unsigned int isPowerOfTwo(uint32_t num);

    int printErrorMsg(const std::string &errorMsg);
    //input parsing functions

    int checkIfArgsValid( char * s1,  char * s2,  char * s3,  char * s4,  char * s5,
                          char * s6);

#endif //CSIM_FNS_H