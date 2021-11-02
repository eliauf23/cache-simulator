/*
 * Source file for cache class.
 * CSF Assignment 3
 * Elizabeth Aufzien eaufzie1@jh.edu
 * Rosie Wolkind rwolkin1@jh.edu
 */

#define MEM_ACCESS_CYCLES 100U
#define MIN_BLOCK_SIZE 4U

#include "block.h"
#include "cache.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <cmath>
#include <iostream>
#include <map>
#include <vector>
#include <typeinfo>

using std::cout;
using std::endl;
using std::map;
using std::string;
using std::vector;

namespace CacheSimulator
{

    Cache::Cache(uint32_t ns, uint32_t nb, uint32_t b_size, Allocation a, Write w, Eviction e)
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

        // initialize cache - using map of pointers (instead of vector) to reduce overhead due to copying & due to constant amortized time complexity for accesses
        sets = new map<uint32_t, vector<Block *> *>;
    }

    // returns Index value from address;
    uint32_t Cache::getIndexFromAddress(uint32_t address) const
    {
        if (_indexLen == 0U)
            return 0U;
        // if fully-assoc cache address = address << _tagLen to  cuts off less significant bits corresponding to offset
        // then address is shifted right to cuts off more significant bits corresponding to tag
        return ((address << _tagLen) >> (32U - _indexLen));
    }

    uint32_t Cache::getTagFromAddress(uint32_t address) const
    {
        // bit shifts right by the length of index & offset to get just the tag
        return (address >> (_indexLen + _offsetLen));
    }

    // Destructor for cache
    // iterates over cache and delete any blocks we've created
    Cache::~Cache()
    {
        for (auto setIter = sets->begin(); setIter != sets->end(); setIter++)
        {
            vector<Block *> *set = setIter->second;
            for (auto blockIter = set->begin(); blockIter != set->end(); blockIter++)
            {
                delete *blockIter;
            }
            delete set;
        }
        delete sets;
    }

    // upon load hit:
    // For LRU - increment block lru counters for set w/ hit. time = LRU time from block that's hit
    // FIFO done implicitly
    void Cache::loadHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        _loadHits++; // increment load hit counter

        if (_evictionType == CacheSimulator::LRU)
        {
            // for LRU, go through set
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                // Increment time for any blocks with time < hit block (should be all but the hit block)
                if ((*iter)->getTime() < hitBlockTime)
                {
                    (*iter)->incrementTime();
                }
                // reset the time for the hit block because we're putting in a new block
                else if ((*iter)->getTime() == hitBlockTime)
                {
                    (*iter)->resetTime();
                }
            }
        }
        // For FIFO, we update the counters when we create the set in load() function,
        // and so there is nothing to do here except update cycles

        // on load hit: increment cycles by 1 bc we move word from cache to cpu
        _cycles++;
    }

    // Check if load hit or load miss given memory address
    void Cache::load(uint32_t index, uint32_t tag)
    {
        // increment loads counter
        _loads++;

        vector<Block *> *set = nullptr;

        // search cache for block given specified address
        if (sets->find(index) != sets->end())
        {
            // load
            set = sets->at(index);
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                // CACHE HIT
                if (tag == (*iter)->getTag())
                {
                    loadHit(set, (*iter)->getTime());
                    return;
                }
            }
            // CACHE MISS & set exists
            loadMissCase2(set, tag);
        }
        else
        {
            // CACHE MISS & set dne. Need to create set and move in block from memory
            loadMissCase1(index, tag);
        }
    }

    // CACHE MISS: Set does not exit & need to create new set
    void Cache::loadMissCase1(uint32_t index, uint32_t tag)
    {
        _loadMisses++; // increments load miss counter

        // create set and add new block to cache
        vector<Block *> *set = new vector<Block *>;
        Block *block = new Block(tag);
        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));

        _cycles++; // increment total cycles by 1

        // add block to set
        set->push_back(block);

        // add set to map of set-index, set *
        sets->insert({index, set});
    }

    // CACHE MISS but set exists
    void Cache::loadMissCase2(vector<Block *> *set, uint32_t tag)
    {
        _loadMisses++; // increment load miss counter

        // increment counter for all the blocks in the set
        for (auto iter = set->begin(); iter != set->end(); iter++)
        {
            (*iter)->incrementTime();
        }

        // if num blocks in vector == num blocks you must evict!
        if (set->size() == _numBlocks)
        {
            // determine which block to evict
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                if ((*iter)->getTime() == _numBlocks)
                {
                    if ((_write == CacheSimulator::WRITE_BACK) && (*iter)->isDirty())
                    {
                        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));
                    }
                    //"evict block": first delete pointer then erase item @ pos of iter to remove block
                    delete *iter;
                    set->erase(iter);
                    break;
                }
            }
            _cycles++; // increment total cycles by 1
        }

        Block *newBlock = new Block(tag);

        // add new block to corresp. set in cache
        set->push_back(newBlock);

        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4)) + 1;
    }

    // on store hit: updates blocks and hitBlockTimes, increments cycles according to cache parameters
    void Cache::storeHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        _storeHits++; // increment counter

        if (_evictionType == CacheSimulator::LRU)
        {
            // iterate through whole set
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                // increment time for all blocks except hitBlock
                if ((*iter)->getTime() < hitBlockTime)
                {
                    (*iter)->incrementTime();
                }
                // when you get to hitBlock, reset the time to 0 because you're writing a new word to the block
                else if ((*iter)->getTime() == hitBlockTime)
                {
                    (*iter)->resetTime();
                    // if it is write back, we mark the block as dirty
                    if (!(_write == CacheSimulator::WRITE_THROUGH))
                    {
                        (*iter)->setDirty(true);
                    }
                }
            }
        }
        _cycles++; // increment cycles

        // if it is write through, we have to write straight to memory so must add 100 to cycles for memory access
        if (_write == CacheSimulator::WRITE_THROUGH)
        {
            _cycles += MEM_ACCESS_CYCLES;
        }
    }

    // Check if store hit or store miss given memory address
    void Cache::store(uint32_t index, uint32_t tag)
    {
        _stores++;

        vector<Block *> *set = nullptr;

        // search the cache for the requested block
        if (sets->find(index) != sets->end())
        {
            set = sets->at(index);
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                // STORE HIT
                if (tag == (*iter)->getTag())
                {
                    storeHit(set, (*iter)->getTime());
                    return;
                }
            }
            // store miss
            _storeMisses++;

            // store miss where set exists logic
            if (_alloc == CacheSimulator::NO_WRITE_ALLOCATE)
            {

                _cycles += MEM_ACCESS_CYCLES;
                //^pretty sure this is only + 100
                // because you can just write 4 byte quantity to memory without bringing it into cache, right?
                return;
            }

            // increment all counters such that you can place new block
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                (*iter)->incrementTime();
            }

            // evict block if nec.
            if (set->size() == _numBlocks)
            {
                for (auto iter = set->begin(); iter != set->end(); iter++)
                {
                    if ((*iter)->getTime() == _numBlocks)
                    {
                        if ((_write == CacheSimulator::WRITE_BACK) && (*iter)->isDirty())
                        {
                            _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));
                        }
                        //"evict block": first delete pointer then erase item @ pos of iter to remove block
                        delete *iter;
                        iter = set->erase(iter);
                        break;
                    }
                }
            }

            // add the new block
            Block *block = new Block(tag);
            set->push_back(block);
            _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));

            // if write through, you have to also write straight to memory and increment by 100
            if (_write == CacheSimulator::WRITE_THROUGH)
            {
                _cycles += MEM_ACCESS_CYCLES;
            }
            else // when its write back, set the dirty flag to be true and increment total cycles by 1 because only updating cache, not memory
            {
                _cycles++;
                block->setDirty(true);
            }
        }

        // STORE MISS
        else
        {
            _storeMisses++;

            // doesn't modify cache
            if (_alloc == CacheSimulator::NO_WRITE_ALLOCATE)
            {
                // only write 4 bytes without bringing entire block into cache
                _cycles += MEM_ACCESS_CYCLES;
                return;
            }

            vector<Block *> *set = new vector<Block *>; // TODO: will need to delete ptr while cleaning up (in destructor?)

            Block *block = new Block(tag);

            // if write allocate: bring block into cache
            _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));

            if (_write == CacheSimulator::WRITE_THROUGH)
            {
                // write 4 bytes to store through to main mem
                _cycles += MEM_ACCESS_CYCLES;
            }

            else
            {
                // defer write, transfer word to cache
                _cycles++;
                block->setDirty(true);
            }
            set->push_back(block);
            sets->insert({index, set});
        }
    }

    void Cache::printResults()
    {
        // prints out results with required formatting
        cout << "Total loads: " << _loads << endl;
        cout << "Total stores: " << _stores << endl;
        cout << "Load hits: " << _loadHits << endl;
        cout << "Load misses: " << _loadMisses << endl;
        cout << "Store hits: " << _storeHits << endl;
        cout << "Store misses: " << _storeMisses << endl;
        cout << "Total cycles: " << _cycles << endl;
    }

    void Cache::printTestingMetrics()
    {
        cout << "Hit rate: " << ((double)(_loadHits + _storeHits) / ((double)(_loads + _stores))) << endl;
        cout << "Miss rate: " << 1 - ((double)(_loadHits + _storeHits) / ((double)(_loads + _stores))) << endl;
        cout << "Miss penalty (cycles): " << (_blockSize / 4) * 100U << endl;
    }

    uint32_t Cache::log_base2(uint32_t num)
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

}