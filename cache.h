#ifndef __CACHE__H__
#define __CACHE__H__
#include "block.h";
#include "set.h";
#include <unordered_map>

namespace CacheSimulator
{

    enum Allocation
    {
        WRITE_ALLOCATE = 0,
        NO_WRITE_ALLOCATE = 1
    };

    enum Write
    {
        WRITE_THROUGH = 0,
        WRITE_BACK = 1
    };

    enum Eviction
    {
        LRU = 0,
        FIFO = 1
    };

    enum Mapping
    {
        DIRECT_MAPPED = 1,
        SET_ASSOCIATIVE = 2,
        FULLY_ASSOCIATIVE = 3
    };

    class Cache
    {

    public:
        Cache(uint32_t num_sets, uint32_t num_blocks, uint32_t block_size, uint32_t miss_strategy, uint32_t write_strategy, uint32_t evict_strategy){};

        //TODO:implement parameterized constructor

        //set functions:
        Set *addSet(std::string index);
        Set *findSet(std::string index);

        //read:
        void readFromCache(Block *block, Set *set, std::string tag);
        int read(std::string index, std::string tag, std::string &firstTag);

        //write:

        //returns int
        uint32_t findAddressInCache(std::string address);

        //preform hash function to see if correlating block has address
        //If yes, return 1
        //If no, return 0
        //Don't update any counters //TODO: check if we should update cycles...But i really don't think so

        //return void
        uint32_t handelStoreHit(std::string address);
        /* {
                //Take in all relevant parameters.
                //Follow logic to update chache/not update
                //Only update cycles. No other parameter
                if (writeAllocate)
                {

                    //TODO: handle write-allocate case
                    //bring the relevant memory block into the cache before the store proceeds
                    //cache.handleWriteMemory(); //overload method <- writing to from memory just increments cycles. do that locally
                }
                else
                { //is no-write allocate
                    //TODO: handle no-write-allocate case by not modifying cache? (Yep, just exit here)
                }
            } */

        //
        uint32_t handleStoreMiss(Cache cache, std::string address);
        /*   {
                if (writeBack)
                {
                    //TODO: handle write-back case
                    //if store & cache hit & write back:
                    //you need to write to cache
                    //mark block as modified: dirty = 1
                    //TODO: make functions to compute the following:
                    // set_index = findIndex(cache, address);
                    //  block_index = findAddressInBlock(cache, address);
                    //if this block is evicted in the future - must be written back to memory before replacing
                }
                else
                { //is write through
                    //TODO: handle write-through case
                    //write to cache & write to memory!
                } */

       // uint32_t handleLoadMiss(cache, address, writeAllocate, writeBack, LIFO, LRU);
        /* {
                    //find corresponding cache block
                    //is there data there?
                    //Yes: Is WB == true?
                    //if yes. Cycles += 100;
                    //regardless, load address to cache.
                    //Mark block as valid.
                    //if writeback, mark as dirty
                    //if writethrough, load to memory (aka Cycles += 100)
                } */

        //functions to initialize cache

    private:
        std::unordered_map <std::string, Set> sets;

        //base fields that will be set from constructor
        uint32_t _currNumSets;
        uint32_t _numSets;
        uint32_t _numBlocks;
        uint32_t _blockSize;

        //calculated fields
        uint32_t _offsetLen;
        uint32_t _indexLen;
        uint32_t _tagLen;
        uint32_t _associativity; //N-way associative

        //cache properties (calculated as well)
        Allocation _alloc;
        Write _write;
        Eviction _evictionType;
        Mapping _mappingType;

        //results!

        uint32_t _loadHits;
        uint32_t _loadMisses;
        uint32_t _storeHits;
        uint32_t _storeMisses;
        uint32_t _cycles;
        //TODO: don;t store as fields and calculate when we print results
        //uint32_t _totalHits = _loadHits + _storeHits;
        // uint32_t _totalMisses = _loadMisses + _storeMisses;
        // uint32_t _numCycles = _totalHits + _totalMisses;

    }; //end of cache class
}
#endif //!__CACHE__H__