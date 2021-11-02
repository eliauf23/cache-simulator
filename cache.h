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


        /*
        * Called for any load operations, it takes in index and tag
        * determines either load miss or load hit, increments cycles, increments counters, and updates cache
        * as appropriate. Also checks all tags (write/no write allocate & write back/through)
        * 
        * Parameters:
        *   index - index value of given address
        *   tag -  unique identifier 
        * 
        * Returns:
        *   void
        */
        void load(uint32_t index, uint32_t tag);
        
        /*
         * Called for any store operation, it uses the index and tag from
         * the memory address, and determines either store hit or store miss.   
         * Then increments cycles and counters, and updates cache according to the
         * tags, either write/no-write allocate & write through/back.
         * 
         * Parameters:
         *  index - index value of given address
         *   tag -  unique identifier 
         * 
         * Returns:
         *   void
        */
        void store(uint32_t index, uint32_t tag);

        /*
        * Called when load(..) determines there is a load hit. 
        * Increments counters (loadHit) & cycles by one .
        * If LRU, updates time counters in the blocks. 
        * 
        * Parameters:
        *   set - the set in which the block to load is in
        *   hitBlockTime - the LRU counter of the block
        * 
        * Returns:
        *   void
        */
        void loadHit(vector<Block *> *set, uint32_t hitBlockTime);


        /*
        * Called when store(..) determines there is a store hit. 
        * Increments counters (storeHit) & cycles according to cache parameters .
        * Updates blocks and hitBlockTimes
        * 
        * Parameters:
        *   set - the set in which the block to load is in
        *   hitBlockTime - the LRU counter of the block
        * 
        * Returns:
        *   void
        */
        void storeHit(vector<Block *> *set, uint32_t hitBlockTime);
   
        /*
        * Called on a loadMiss, when the set from which youd expect to load the block from is empty
        * Creates set and moves in block from memory. Updates counters and cycles.
        * Then adds set to the cache.
        *
        * Parameters:
        *       index - index value of given address
        *       tag -  unique identifier  
        *  Returns:
        *   void       
        */ 
        void loadMissCase1(uint32_t index, uint32_t tag);
    
        /*
        * Called on a loadMiss, when the set from which youd expect to load the block from exists
        * but is not storing the given address in any of its block. 
        * Increments the LRU counter for all the blocks, and then updates the value in either the FI/LRU block. 
        * Updates counters and cycles. 
        *
        * Parameters:
        *       index - index value of given address
        *       tag -  unique identifier   
        *  Returns:
        *   void      
        */         
       void loadMissCase2(vector<Block *> *set, uint32_t tag);


        static uint32_t log_base2(uint32_t num);

        /*
        * Prints out all the counters per the assignment specifications.
        *
        *  Parameters:
        *   none
        * 
        *  Returns:
        *   void
        */
        void printResults();

        /*
        * Utilizes bit shifting to retrieve index from an address value
        * 
        * Parameters:
        *   address - memory address
        * 
        * Returns:
        *   index value of address as an uint32_t
        */
        uint32_t getIndexFromAddress(uint32_t address) const;


        /*
        * Utilizes bit shifting to retrieve tag from an address value
        * 
        * Parameters:
        *   address - memory address
        * 
        * Returns:
        *   tag value of address as an uint32_t
        */
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