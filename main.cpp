#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include "set.h"
#include <string>
#include <cstring>
#include <sstream>
using namespace std;

//TODO: clarify where we need to use uint32_t vs unsigned int?

int main(int argc, char ** argv) {

    //get num args:

    if (argc != 7) {
        return printErrorMsg("Invalid cmd line args.");
    } else {
    uint32_t numSets = 0U;
    uint32_t numBlocks = 0U;
    uint32_t blockSize = 0U;

    //modularize all of the following into argument parsing/checking functions
    if (argv[1] != NULL && atol(argv[1]) > 0) {
        sscanf(argv[1], "%u", &numSets);
        //check if not power of two
        if (!isPowerOfTwo(numSets)) {
            return printErrorMsg("Num sets is not power of 2.");
        }
    } else {
        return printErrorMsg("Invalid cache features.");

    }

    //determine if block number is valid
    if (argv[2] != NULL && atol(argv[2]) > 0) {
        sscanf(argv[2], "%u", &numBlocks);
        if (!isPowerOfTwo(numBlocks)) {
            //check if not power of two
            return printErrorMsg("Num blocks is not power of 2.");

        }
    } else {
       return printErrorMsg("Invalid cache features.");
    }

    if (argv[3] != NULL && atol(argv[3]) > 0) {
        sscanf(argv[3], "%u", &blockSize);
        if (!isPowerOfTwo(blockSize)) {
            //check if not power of two
            return printErrorMsg("cache size not power of 2.");

        }
    } else {
        return printErrorMsg("Invalid cache features.");
    }

    unsigned int writeAllocate = 0U;
    unsigned int writeBack = 0U;
    // unsigned int lru = 0U;
    // unsigned int fifo = 0U;

    if (strcmp("no-write-allocate", argv[4]) == 0) {
        writeAllocate = 0U;
    } else if (strcmp("write-allocate", argv[4]) == 0) {
        writeAllocate = 1U;
    } else {
        return printErrorMsg("Problem with arg 4");
    }

    if (strcmp("write-through", argv[5]) == 0) {
        writeBack = 0U;
    } else if (strcmp("write-back", argv[5]) == 0) {
        writeBack = 1U;
    } else {
        return printErrorMsg("Problem with arg 5");
    }

//don't need yet - commented out so don't get warning unused var
    if (strcmp("fifo", argv[6]) == 0) {
        lru = 0;
        fifo = 1;
    } else if (strcmp("lru", argv[6]) == 0) {
        lru = 1;
        fifo = 0;
    } else {
        return printErrorMsg("Problem with arg 6");
    }

    // error checking
    if (blockSize < MIN_BLOCK_SIZE) {
       printf("%u", blockSize);
        return printErrorMsg("Invalid block size");

    }
    if (!writeAllocate && writeBack) {
        return printErrorMsg("Contradictory arguments");
    }


    //make cache

    Cache cache = new Cache(/* fill out init fields*/);
    cache.numSets = numSets;
    // //implement log base2 function or get it from math library
    cache.indexLen = log_base2(blockSize);
    cache.offsetLen = log_base2(numSets);
    cache.tagLen = 32- (cache.offsetLen + cache.indexLen);

    //while(getline(&std::cin, line) )


    /* //can use scanf with stdin to get trace lines
    char storeOrLoad;
    uint32_t address;
    int third_thing; //TODO: we don't need this, right
    //want to consume third thing and move onto next line -> does it automatically parse it?
    while(scanf(" %c %x %d ", &storeOrLoad, &address, &third_thing) == 3) {
        //yes =1 , no = 0;
        int isDataInCache = findAddressInCache(cache, address);

        if(storeOrLoad == 's') {
            stores++;
            if(isDataInCache == 1) {
                storeHits++;
                return;
            }
            if(isDataInCache == 0) { //cache miss
                storeMisses++;
                //pass in all relevant parameter. Only updates Cycles!
                cache.handelStoreHit(cache, address, writeAllocate, writeBack);
                
            }
            else {  //if cache hit(should increment loadHits //TODO: should you? Its store here not load!)
                storeHits++;
                cache.handleStoreMiss(cache, address, writeAllocate, writeBack)
               }
        } 
        
        else if(storeOrLoad == 'l') {
            loads++;
           if(isDataInCache == 0) {
               loadMisses++;
               cache.handleLoadMiss(cache, address, writeAllocate, writeBack, LIFO, LRU);
               //loadMissingData();
               //cycles += numCyclesToLoadData + 1;
           } else if(isDataInCache == 1) {//cache hit
               loadHits++;
                cycles++;
                //update num uses of cache blocks
           }
        } else {
            return printErrorMsg("trace file had invalid operation.");
        }
    } */
    //output results
    //displayResults(loads, stores, loadHits, loadMisses, storeHits, storeMisses, cycles);



    // free any memory allocated in cache & other cleanup
   // free(cache.sets);
    //for(unsigned int i = 0; i < numSets; i++) {
    //    free((cache.sets)[i].blocks);
    //}
    return 0; //success!
    }
}


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


 error handling specs:
- block size is not a power of 2
- number of sets is not a power of 2
- block size is less than 4
 - write-back and no-write-allocate were both specified
 - If the configuration parameters are invalid, the program should

Print an error message to stderr or std::cerr, and
Exit with a non-zero exit code
*/
