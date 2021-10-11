#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <string.h>
#include "csim_fns.h"




int main(int argc, char ** argv) {

//  uint32_t MIN_BLOCK_SIZE = 4U;
    /* get input file

    parse each memory access: field1 field2 field3
    field1= l or s
    field2=32 bit memory address in hex
    field 3(ignore)
    assume each load/store operation accesses at most 4 bytes of data & no load/store accesses data that spans more than 1 cache block (i.e. lines)

    cache sim takes following cmd line args 
    
    - num sets in cache (pos pow of 2)
    - num of blocks in each set (pos pow of 2)
    - num of bytes in each block;  (pos pow of 2, at least 4)
    - write-allocate OR no-write-allocate
    - write-through OR write-back
    - lru (least-recently-used) or fifo evictions
    
    */

    //get num args:

    if (argc != 7) {
        fprintf(stderr, "Invalid cmd line args.\n");
        return 1;
    } else {
    
    uint32_t numSets = 0;
    uint32_t numBlocks = 0;
    uint32_t blockSize = 0;

    //turn this into function "assert valid cache features"    assertValidCacheFeature();


    if (argv[1] != NULL && atol(argv[1]) > 0) {
        sscanf(argv[1], "%u", &numSets);
        if (!(numSets && !(numSets & (numSets - 1)))) {
            //check if not power of two
            fprintf(stderr, "cache size not power of 2.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid cache features.\n");
        return 1;
    }

    //determine if block number is valid
    if (argv[2] != NULL && atol(argv[2]) > 0) {
        sscanf(argv[2], "%u", &numBlocks);
        if (!(numBlocks && !(numBlocks & (numBlocks - 1)))) {
            //check if not power of two
            fprintf(stderr, "cache size not power of 2.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid cache features.\n");
        return 1;
    }


    if (argv[3] != NULL && atol(argv[3]) > 0) {
        sscanf(argv[3], "%u", &blockSize);
        if (!(blockSize && !(blockSize & (blockSize - 1)))) {
            //check if not power of two
            fprintf(stderr, "cache size not power of 2.\n");
            return 1;
        }
    } else {
        fprintf(stderr, "Invalid cache features.\n");
        return 1;
    }

    uint32_t writeAllocate = 0;
    uint32_t writeBack = 0;
    uint32_t lru = 0;
    uint32_t fifo = 0;

    if (strcmp("no-write-allocate", argv[4]) == 0) {
        writeAllocate = 0;
    } else if (strcmp("write-allocate", argv[4]) == 0) {
        writeAllocate = 1;
    } else {
        fprintf(stderr, "Problem with arg 4\n");
        return 1;
    }

    if (strcmp("write-through", argv[5]) == 0) {
        writeBack = 0;
    } else if (strcmp("write-back", argv[5]) == 0) {
        writeBack = 1;
    } else {
        fprintf(stderr, "Problem with arg 5\n");
        return 1;
    }

    if (strcmp("fifo", argv[6]) == 0) {
        lru = 0;
        fifo = 1;
    } else if (strcmp("lru", argv[6]) == 0) {
        lru = 1;
        fifo = 0;
    } else {
        fprintf(stderr, "Problem with arg 6\n");
        return 1;
    }

    // error checking
    if (blockSize < MIN_BLOCK_SIZE) {
        printf("%u", blockSize);
        fprintf(stderr, "Invalid block size\n");
        return 1;
    }
    if (!writeAllocate && writeBack) {
        fprintf(stderr, "Contradictory arguments\n");
        return 1;
    }

    uint32_t loads = 0;
    uint32_t stores = 0;
    uint32_t loadHits = 0;
    uint32_t loadMisses = 0;
    uint32_t storeHits = 0;
    uint32_t storeMisses = 0;
    uint32_t cycles = 0;


    printf("Total loads: %u\nTotal stores: %u\nLoad hits: %u\nLoad misses: %u\nStore hits: %u\nStore misses: %u\nTotal cycles: %u\n", 
    loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);

    // free any memory allocated in cache
    return 0;
    }

    
}