

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#define MIN_BLOCK_SIZE 4U


#include <iostream>
#include <string>
#include <cstring>

    // parses integer from input str if greater than 0 or a power of 2
    // if error(aka if  a cache miss): returns -1
    // if not a cache miss, returns index in cache
    int getValidInteger(std::string str);

// returns 0 if uint32_t is a power of two
// else returns 1;
unsigned int isPowerOfTwo(uint32_t num);

    // takes in error message
    //prints out msg to std:err
    int printErrorMsg(const std::string &errorMsg);


    int checkIfArgsValid( char * s1,  char * s2,  char * s3,  char * s4,  char * s5,
                          char * s6);
    //goes through the six args and returns 0 if they are individually valid && no illegal combinations
    //returns 1 & prints an error message if any of the args are invalid or if there are illegal combinations 

#endif //CSIM_FNS_H