#ifndef CACHE_H
#define CACHE_H

#include "set.h"
#include "block.h"
#include <string>
#include <cstdint>
#include <climits>
#include <unordered_map>

using std::cout;
using std::endl;
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

    class Cache
    {

    public:
        Cache() = default;

        ~Cache() {}

        Cache(uint32_t ns, uint32_t nb, uint32_t b_size, Allocation a, Write w, Eviction e)
        {
            _numSets = ns;
            _numBlocks = nb;
            _blockSize = b_size;
            _alloc = a;
            _write = w;
            _evictionType = e;
            _currNumSets = 0;
            _loadHits = 0;
            _loadMisses = 0;
            _storeHits = 0;
            _storeMisses = 0;
            _cycles = 0;

            _offsetLen = log_base2(_blockSize);
            _indexLen = log_base2(_numSets);
            _tagLen = 32 - (_offsetLen + _indexLen);

            // initialize cache
            for (uint32_t i = 0; i < ns; i++)
            {
                addSet(i);
            }
        };

        bool isLRU() const;
        bool isFIFO() const;
        bool isWriteBack() const;
        bool isWriteAllocate() const;
        bool isWriteThrough() const;

        static uint32_t log_base2(uint32_t num)
        {
            uint32_t result = 0U;

            if (num == 0)
                return UINT_MAX;
            else if (num == 1)
                return 0;
            else
            {
                while (num > 1)
                {
                    num = num >> 1;
                    result++;
                }
                return result;
            }
        }

        // set functions:
        void addSet(uint32_t index);

        Set *findSet(uint32_t index);

        int evictBlock(uint32_t index, uint32_t tag);

        void writeBack(uint32_t index, uint32_t tag);

        void writeAllocate(uint32_t index, uint32_t tag);

        void writeThrough(uint32_t index, uint32_t tag);

        uint32_t getIndexFromAddress(uint32_t address) const;

        uint32_t getTagFromAddress(uint32_t address) const;

        bool checkIfCacheHit(uint32_t index, uint32_t tag);

        void printResults();

        void handleStoreHit(uint32_t index, uint32_t tag);

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

        void handleStoreMiss(uint32_t index, uint32_t tag);

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

        void handleLoadMiss(uint32_t index, uint32_t tag);
        /* {
                    //find corresponding cache block
                    //is there data there?
                    //Yes: Is WB == true?
                    //if yes. Cycles += 100;
                    //regardless, load address to cache.
                    //mark block as valid.
                    //if writeback, mark as dirty
                    //if writethrough, load to memory (aka Cycles += 100)
                } */

        // functions to initialize cache

        void handleLoadHit(uint32_t index, uint32_t tag);

        // results!

        void incLoads();
        void incStores();

        void incLoadHits();

        void incLoadMisses();

        void incStoreHits();

        void incStoreMisses();

        void cacheToCpuOperation();

        void memoryToCacheOperation();

        uint32_t _loads = 0U;
        uint32_t _stores = 0U;
        uint32_t _loadHits = 0U;
        uint32_t _loadMisses = 0U;
        uint32_t _storeHits = 0U;
        uint32_t _storeMisses = 0U;
        uint32_t _cycles = 0U;

    public:
        uint32_t getNumBlocks() const;
        std::unordered_map<uint32_t, Set> sets;

    private:
        // base fields that will be set from constructor
        uint32_t _currNumSets = 0U;
        uint32_t _numSets = 0U;
        uint32_t _numBlocks = 0U;
        uint32_t _blockSize = 0U;

        // calculated fields
        uint32_t _offsetLen = 0U;
        uint32_t _indexLen = 0U;
        uint32_t _tagLen = 0U;

        // cache properties (calculated as well)
        Allocation _alloc;
        Write _write;
        Eviction _evictionType;
    };
    // end of cache class
}
#endif // CACHE_H