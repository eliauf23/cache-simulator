/* assumptions from assignment description:
 * this is number of cycles to perform
 * load/store operation from specified location
 * to CPU for a single 4 byte quantity
 */
#define CACHE_ACCESS_CYCLES 1U
#define MEM_ACCESS_CYCLES 100U

#define MIN_BLOCK_SIZE 4U

#include "block.h"
#include "cache.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <unordered_map> //using instead of vector bc of constant amortized time complexity
#include <cmath>
#include <iostream>
#include <map>
#include <vector>

using std::cout;
using std::endl;
using std::map;
using std::vector;

using std::cout;
using std::endl;
using std::string;

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

        // initialize cache
        sets = new map<uint32_t, vector<Block *> *>;
    }

    uint32_t Cache::getBlockSize() const
    {
        return _blockSize;
    }

    uint32_t Cache::getNumBlocks() const
    {
        return _numBlocks;
    }

    uint32_t Cache::getIndexFromAddress(uint32_t address) const
    {
        if (_indexLen == 0U)
            return 0U;                       // if fully-assoc cache
        address = address << _tagLen;        // this cuts off more significant bits corresponding to tag
        return address >> (32U - _indexLen); // this cuts off less significant bits corresponding to offset
    }

    uint32_t Cache::getTagFromAddress(uint32_t address) const
    {
        return address >> (_indexLen + _offsetLen);
    }

    // Destructor for cache
    // need to iterate over cache and delete block's we've created
    Cache::~Cache()
    {
        for (map<uint32_t, vector<Block *> *>::iterator setIter = sets->begin(); setIter != sets->end(); setIter++)
        {
            
            vector<Block *> *set = setIter->second;
            for (vector<Block *>::iterator blockIter = set->begin(); blockIter != set->end(); blockIter++)
            {
                delete *blockIter;
            }
            delete set;
        }
        delete sets;
    }
    // upon load hit - increment block lru counters for set w/ hit
    // time = LRU time from block that's hit

    void Cache::loadHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        if (isLRU())
        {
            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                if ((*iter)->getTime() < hitBlockTime)
                {
                    (*iter)->incrementTime();
                }
                else if ((*iter)->getTime() == hitBlockTime)
                {
                    (*iter)->resetTime();
                }
            }
        }

        //on load hit - only inc. cycles by 1 when move word from cache to cpu
        cacheToCpuOperation();
    }

    // Check if load hit or load miss given memory address
    void Cache::load(uint32_t address)
    {
        // increment loads counter
        incLoads();
        // get index and tag from address
        uint32_t index = getIndexFromAddress(address);
        uint32_t tag = getTagFromAddress(address);

        vector<Block *> *set;
        // search cache for block given specified address
        if (sets->find(index) != sets->end())
        {
            set = sets->at(index);
            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                if (tag == (*iter)->getTag())
                {
                    // CACHE HIT
                    incLoadHits();
                    loadHit(set, (*iter)->getTime());
                    return;
                }
            }
            // CACHE MISS
            loadMissCase2(set, tag);
        }
        else
        {
            // a miss has occurred
            loadMissCase1(index, tag);
        }
        incLoadMisses();
    }

    // create new set
    void Cache::loadMissCase1(uint32_t index, uint32_t tag)
    {
        // create set and add new block to cache
        cout << "Entered loadMissCase1" << endl;

        vector<Block *> *set = new vector<Block *>; // TODO: will need to delete ptr while cleaning up (in destructor?)
        Block *block = new Block(tag);              // TODO: will need to delete ptr while cleaning up (in destructor?)
        memoryToCacheOperation();

        cacheToCpuOperation();

        // add block to set
        set->push_back(block);
        // add set to map of set-index, set *

        sets->insert({index,
                      set});
    }
    // dont need to create new set
    void Cache::loadMissCase2(vector<Block *> *set, uint32_t tag)
    {
        // case 1: load miss where set already exists
        cout << "Entered loadMissCase2" << endl;

        for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
        {
            (*iter)->incrementTime();
        }

        // if num blocks in vector == num blocks you must evict!
        if (set->size() == _numBlocks)
        {
            // determine which block to evict

            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                if ((*iter)->getTime() == _numBlocks)
                {
                    if (!isWriteThrough() && (*iter)->isDirty())
                    {
                        //write back must always 
                        memoryToCacheOperation();
                    }
                    delete *iter; // todo: check all deletes
                    // remove block by erasing item @ position of iterator
                    set->erase(iter);
                    break;
                }
            }
            cacheToCpuOperation();
        }
        Block *newBlock = new Block(tag); // TODO: will need to delete ptr while cleaning up (in destructor?)
        // add new block to corresp. set in cache

        set->push_back(newBlock);
        cout << "XOXO" << endl;
        memoryToCacheOperation();
        cacheToCpuOperation();
    }

    // on store hit: updates blocks and hitBlockTimes, increments cycles according to cache parameters
    void Cache::storeHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        if (isLRU())
        {
            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                if ((*iter)->getTime() < hitBlockTime)
                {
                    (*iter)->incrementTime();
                }
                else if ((*iter)->getTime() == hitBlockTime)
                {
                    (*iter)->resetTime();
                    if (!isWriteThrough())
                    {
                        (*iter)->setDirty(true);
                    }
                }
            }
        }
        cacheToCpuOperation();
        if (isWriteThrough())
        {
            write4bytesToMemory(); // TODO: pretty sure this is only + 100
        }
    }

    // Check if store hit or store miss given memory address

    void Cache::store(uint32_t address)
    {
        incStores();
        // get relevant pieces of address
        uint32_t index = getIndexFromAddress(address);
        uint32_t tag = getTagFromAddress(address);

        vector<Block *> *set;
        // search the cache for the requested block
        if (sets->find(index) != sets->end())
        {
            set = sets->at(index);
            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                if (tag == (*iter)->getTag())
                {
                    // STORE HIT
                    incStoreHits();
                    storeHit(set, (*iter)->getTime());
                    return;
                }
            }
            // store miss
            incStoreMisses();

            // store miss where set exists logic
            if (!isWriteAllocate())
            {

                write4bytesToMemory();
                //^pretty sure this is only + 100
                // because you can just write 4 byte quantity to memory without bringing it into cache, right?
                return;
            }

            // increment all counters such that you can place new block

            for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
            {
                (*iter)->incrementTime();
            }
            // evict block if nec.
            if (set->size() == _numBlocks)
            {
                for (vector<Block *>::iterator iter = set->begin(); iter != set->end(); iter++)
                {
                    if ((*iter)->getTime() == _numBlocks)
                    {
                        if (!isWriteThrough() && (*iter)->isDirty())
                        {
                            memoryToCacheOperation();
                        }
                        delete *iter; // todo check all delets
                        iter = set->erase(iter);
                        break;
                    }
                }
            }
            // add the new block
            Block *block = new Block(tag); // TODO: will need to delete ptr while cleaning up (in destructor?)
            set->push_back(block);
            memoryToCacheOperation();
            if (isWriteThrough())
            {
                write4bytesToMemory(); // TODO: write 4 bytes?
            }
            else
            {
                cacheToCpuOperation();
                block->setDirty(true);
            }
        }

        // STORE MISS
        else
        {
            incStoreMisses();
            if (!isWriteAllocate())
            {
                write4bytesToMemory(); // TODO: write 4 bytes?
                return;
            }
            vector<Block *> *set = new vector<Block *>; // TODO: will need to delete ptr while cleaning up (in destructor?)

            Block *block = new Block(tag);
            memoryToCacheOperation();

            if (isWriteThrough())
            {
                write4bytesToMemory();
            }
            else
            {
                cacheToCpuOperation();
                block->setDirty(true);
            }
            set->push_back(block);
            sets->insert({index,
                          set});
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

    void Cache::incLoads()
    {
        _loads++;
    }
    void Cache::incStores()
    {
        _stores++;
    }

    void Cache::incLoadHits()
    {
        _loadHits++;
    }

    void Cache::incLoadMisses()
    {
        _loadMisses++;
    }

    void Cache::incStoreHits()
    {
        _storeHits++;
    }

    void Cache::incStoreMisses()
    {
        _storeMisses++;
    }

    void Cache::cacheToCpuOperation()
    {

        _cycles++;
    }

    void Cache::memoryToCacheOperation()
    {
        cout << getBlockSize() << " " << "cycles: " << (100 * (getBlockSize() / 4)) << endl;

        _cycles += (MEM_ACCESS_CYCLES * _blockSize / 4);
    }

    void Cache::write4bytesToMemory()
    {

        _cycles += (100);
    }

    bool Cache::isLRU() const
    {
        return _evictionType == CacheSimulator::LRU;
    }
    bool Cache::isFIFO() const
    {
        return _evictionType == CacheSimulator::FIFO;
    }
    bool Cache::isWriteBack() const
    {
        return _write == CacheSimulator::WRITE_BACK;
    }
    bool Cache::isWriteAllocate() const
    {
        return _alloc == CacheSimulator::WRITE_ALLOCATE;
    }
    bool Cache::isWriteThrough() const
    {
        return _write == CacheSimulator::WRITE_THROUGH;
    }
}