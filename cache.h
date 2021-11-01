#ifndef CACHE_H
#define CACHE_H

#include "block.h"
#include <string>
#include <cstdint>
#include <climits>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::vector;

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
        //pointer to map of unsigned 32-bit int, pointer to vector of block pointers...
        map<uint32_t, vector<Block *> *> *sets;

        Cache() = default;

       ~Cache();

        Cache(uint32_t ns, uint32_t nb, uint32_t b_size, Allocation a, Write w, Eviction e);

        void load(uint32_t index, uint32_t tag);
        void store(uint32_t index, uint32_t tag);

        void loadHit(vector<Block *> *set, uint32_t hitBlockTime);
        void storeHit(vector<Block *> *set, uint32_t hitBlockTime);
   
        //c1: create new set
        void loadMissCase1(uint32_t index, uint32_t tag);
    
        //c2: dont need to create new set - must pass in existing set pointer as argument
        void loadMissCase2(vector<Block *> *set, uint32_t tag);


        static uint32_t log_base2(uint32_t num);

        
        void printResults();

        //uses bit shifting to get the index from the address;
        uint32_t getIndexFromAddress(uint32_t address) const;

        //uses bit shifting to get the tag from the address;
        uint32_t getTagFromAddress(uint32_t address) const;

    private:

        bool isLRU() const;
        bool isFIFO() const;
        bool isWriteBack() const;
        bool isWriteAllocate() const;
        bool isWriteThrough() const;
        void write4bytesToMemory();

        // functions to keep track of cache simulator numbers
        void incLoads();
        void incStores();
        void incLoadHits();
        void incLoadMisses();
        void incStoreHits();
        void incStoreMisses();
        void cacheToCpuOperation();
        void memoryToCacheOperation();
        uint32_t getBlockSize() const;
        uint32_t getNumBlocks() const;

        uint32_t _loads = 0U;
        uint32_t _stores = 0U;
        uint32_t _loadHits = 0U;
        uint32_t _loadMisses = 0U;
        uint32_t _storeHits = 0U;
        uint32_t _storeMisses = 0U;
        uint32_t _cycles = 0U;

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