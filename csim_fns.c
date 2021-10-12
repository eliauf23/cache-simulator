#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include <limits.h>
#include "csim_fns.h"


int printErrorMsg(char * msg) {
    fprintf(stderr, "%s\n", msg);
    return 1; //exit or return?
}

void displayResults(uint32_t loads, uint32_t stores, uint32_t loadHits, uint32_t loadMisses, uint32_t storeHits, uint32_t storeMisses, uint32_t cycles) {
    printf("Total loads: %u\nTotal stores: %u\nLoad hits: %u\nLoad misses: %u\nStore hits: %u\nStore misses: %u\nTotal cycles: %u\n",
           loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);
}


unsigned int isPowerOfTwo(uint32_t num) {
    return (num && !(num & (num - 1)));
}

int findAddressInCache(Cache cache, uint32_t address){
    int something = cache.numSets + address;
    something = -1;
    return something;
}

unsigned int log_base2(unsigned int num) {
    if (num == 0) return UINT_MAX; //TODO: I think this is preferable to using negatives. Thoughts?
    if (num == 1) return 0;
    unsigned int result = 0U;
    while (num > 1) {
        num = num >> 1;
        num++;
    }
    return result;
}
