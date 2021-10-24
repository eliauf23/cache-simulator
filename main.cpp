#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include "set.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>

#define NUM_ARGS 7

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::stoul;
using std::string;

int main(int argc, char *argv[])
{

    // ensures no illegal combinations of args & all valid

    if (argc != NUM_ARGS)
    {
        return printErrorMsg("incorrect num args");
    }
    else if (checkIfArgsValid(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]) != 0)
    {
        return printErrorMsg("Invalid args. Try again.");
    }
    else
    {

        // parse all command line args
        uint32_t numSets = getValidInteger(argv[1]);
        uint32_t blocksPerSet = getValidInteger(argv[2]);
        uint32_t blockSize = getValidInteger(argv[3]);
        // enums from cache.cpp
        CacheSimulator::Allocation alloc;
        CacheSimulator::Write write;
        CacheSimulator::Eviction evict;

        if (strcmp(argv[4], "no-write-allocate") == 0)
            alloc = CacheSimulator::NO_WRITE_ALLOCATE;
        else
            alloc = CacheSimulator::WRITE_ALLOCATE;

        if (strcmp(argv[5], "write-through") != 0)
            write = CacheSimulator::WRITE_THROUGH;
        else
            write = CacheSimulator::WRITE_BACK;

        if (strcmp(argv[6], "fifo") != 0)
            evict = CacheSimulator::FIFO;
        else
            evict = CacheSimulator::LRU;

        // initialize cache
        CacheSimulator::Cache cache = CacheSimulator::Cache(numSets, blocksPerSet, blockSize, alloc, write, evict);

        string operation(""); // load or store
        uint32_t address;      // hex address
        string line("");

        // read in file

        while (getline(std::cin, line))
        {
            operation = line.substr(0, 1);
            address = stoul(line.substr(4, 12), 0, 16);

            // simulate

            uint32_t index = cache.getIndexFromAddress(address);
            uint32_t tag = cache.getTagFromAddress(address);

            uint32_t blockIdx = cache.getBlockIndex(index, tag);
            // get address of block added

            // STORE!
            if (operation == "s")
            {
                if (blockIdx != cache.getNumBlocks())
                { // cache miss
                    if (cache.isWriteAllocate())
                    {                                                        // load value into cache and change it there, implies writeBack
                        cache.loadToCache(index, tag);                       // loading value from main, cache.loadToCache sets lru and fifo counters
                        cache._cycles++;                                     // writing to value in cache
                        uint32_t blockIdx = cache.getBlockIndex(index, tag); // get address of block added
                        if (cache.isWriteBack())
                        {
                            cache.sets[index]._blocks[blockIdx].setDirty(true);
                        }
                        else
                        {
                            cache._cycles += 100; // write to main because store miss and write allocate
                        }
                    }
                    else
                    { // no-write-allocate - write value straight to main memory (skip over cache)
                        cache._cycles += 100;
                    }
                    cache._storeMisses++;
                }

                else
                { // cache hit, blockIdx contains block number
                    if (cache.isWriteBack())
                    { // change value in cache, make sure to mark dirty bit
                        cache.sets[index]._blocks[blockIdx].setDirty(true);
                        cache._cycles++;
                    }
                    else
                    {                         // write-through - change value in cache and in main memory
                        cache._cycles += 100; // update value in main memory
                    }
                    cache._storeHits++;
                    if (cache.isLRU())
                    {
                        CacheSimulator::Set * s = cache.findSet(index);
                        s->incrementLRU(blockIdx);
                        delete s;
                    }
                }

                // increment stores regardless
                cache._stores++;
            }

            // LOAD!

            else if (operation == "l")
            {
                uint32_t blockIdx = cache.getBlockIndex(index, tag);
                if (blockIdx == cache.getNumBlocks())
                { // cache miss
                    // need to load value into cache
                    cache._loadMisses++;

                    cache.loadToCache(index, tag); // cache.loadToCache updates fifo and lru counters
                    cache._cycles++;
                }
                else
                {

                    cache._cycles++;
                    cache._loadHits++;
                    if (cache.isLRU())
                    {
                        CacheSimulator::Set * s = cache.findSet(index);
                        s->incrementLRU(blockIdx);
                        delete s;
                    }
                }

                // increment loads regardless
                cache._loads++;
            }

            // ERROR!

            else
            {
                return printErrorMsg("issue with trace file");
            }
        }
        cache.printResults();


        return 0;
    }
}