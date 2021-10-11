#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>



#define MIN_BLOCK_SIZE 4U

//input parameters are num sets = s, num blocks = b, num bytes per block= block_size
//



//use unsigned 32 bit integers for any memory addresses/indices
typedef struct {

    uint32_t isValid;
    uint32_t size;
    uint32_t tag;
    uint32_t * data; //Array of bytes (array size = size)

} Block;

typedef struct {
        uint32_t numBlocks; //blocks per set
        

} Set;

typedef struct {
    //properties
    uint32_t num_sets;
    Set * cache_sets;

} Cache;


//do we want to use c++ so we can define classes?


#endif
