#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include "set.h"
#include <string>
#include <cstdint>
#include <iostream>

#define NUM_ARGS 7

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::stoul;
using std::string;

int main(int argc, char **argv)
{
    string s1 = string(argv[1]);
    string s2 = string(argv[2]);
    string s3 = string(argv[3]);
    string s4 = string(argv[4]);
    string s5 = string(argv[5]);
    string s6 = string(argv[6]);

    // ensures no illegal combinations of args & all valid
    bool argsValid = CacheSimulator::checkIfArgsValid(s1, s2, s3, s4, s5, s6) == 0;

    if (argc == NUM_ARGS && argsValid)
    {
        // parse all command line args
        uint32_t numSets = CacheSimulator::getValidInteger(s1);
        uint32_t blocksPerSet = CacheSimulator::getValidInteger(s2);
        uint32_t blockSize = CacheSimulator::getValidInteger(s3);
        // enums from cache.cpp
        CacheSimulator::Allocation alloc;
        CacheSimulator::Write write;
        CacheSimulator::Eviction evict;

        if (s4 == "no-write-allocate")
            alloc = CacheSimulator::NO_WRITE_ALLOCATE;
        else
            alloc = CacheSimulator::WRITE_ALLOCATE;

        if (s5 == "write-through")
            write = CacheSimulator::WRITE_THROUGH;
        else
            write = CacheSimulator::WRITE_BACK;

        if (s6 == "fifo")
            evict = CacheSimulator::FIFO;
        else
            evict = CacheSimulator::LRU;

        // initialize cache
        CacheSimulator::Cache cache = CacheSimulator::Cache(numSets, blocksPerSet, blockSize, alloc, write, evict);

        string operation = ""; // load or store
        uint32_t address;      // hex address
        string line;

        // read in file
        while (getline(cin, line))
        {
            operation = line.substr(0, 1);
            address = stoul(line.substr(4, 12), 0, 16);

            uint32_t index = cache.getIndexFromAddress(address);
            uint32_t tag = cache.getTagFromAddress(address);
            bool cacheHit = cache.checkIfCacheHit(index, tag);

            //STORE HIT
            if (operation == "s" && cacheHit)
            {
                cache.incStoreHits();
                cache.handleStoreHit(index, tag);
            }
            //STORE MISS
            else if (operation == "s" && !cacheHit)
            {
                cache.incStoreMisses();
                cache.handleStoreMiss(index, tag);
            }

            //LOAD HIT
            else if (operation == "l" && cacheHit)
            {
                cache.incLoadHits();
                cache.handleLoadHit(index, tag);
            }

            //LOAD MISS
            else if (operation == "l" && !cacheHit)
            {
                
                cache.incLoadMisses();
                cache.handleLoadMiss(index, tag);
            }
            else
            {
                // error!
                return CacheSimulator::printErrorMsg("Invalid operation");
                exit(1);
            }
        }
        cache.printResults();

        return 0;
    }
    else
    {
        return CacheSimulator::printErrorMsg("Invalid cmd line args.");
    }
}