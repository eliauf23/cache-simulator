#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define MIN_BLOCK_SIZE 4U

//use unsigned 32 bit integers for any memory addresses/indices
typedef struct {
    uint32_t dirty; //dirty bit - for write-back cache (tells you that you need to write changes back to memory before evicting block)
    uint32_t valid; //valid bit
    uint32_t tag;//tag - tells you the block you're in
    uint32_t evictionPriority; //higher value indicates that it's not being accessed
    // for LRU: measure of times set has been accessed, but this block hasn't been
} Block;

typedef struct {
        uint32_t dummyVar;

} Set;

typedef struct {
        uint32_t dummyVar;

} Cache;


//do we want to use c++ so we can define classes?


#endif
