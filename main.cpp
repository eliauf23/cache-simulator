#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include "set.h"
#include <string>
#include <cstring>
#include <sstream>

#define NUM_ARGS 8

using namespace std;

//TODO: clarify where we need to use uint32_t vs unsigned int?

int main(int argc, char ** argv) {

    /*
    
    //get num args:

    if (argc != NUM_ARGS) {
        return printErrorMsg("Invalid cmd line args.");
    } else {

        //check if arguments are valid & parse them:
        //must be unsigned int, greater than 0 & a power of 2
        getValidInteger(argv[1]); 
        getValidInteger(argv[2]);
        getValidInteger(argv[3]);
        //parse 

    
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

    */


    //make cache


    std::string line;
    std::string operation; //load or store
    uint32_t address; //hex address
    uint32_t unusedNum; //thing @ end of the line

    getline(std::cin, line);
    while(!std::cin.eof()) {
        //create string stream to extract info from line
        std::istringstream line_stream(line);
        //split line into "operation | address (hex num) | unusedNum (decimal num)"
        line_stream >> operation;
        line_stream >> std::hex >> address;
        line_stream >> std::dec >> unusedNum;

        std::cout << operation << " " << address << std::endl;
        return 0;

        //do stuff for line -> read/write



        //get next line
        getline(std::cin, line);

    }

    //break out of loop: terminate & print results or error handling stuff
    

    return 0;    
}