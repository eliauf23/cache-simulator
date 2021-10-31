/* assumptions from assignment description:
 * this is number of cycles to perform
 * load/store operation from specified location
 * to CPU for a single 4 byte quantity
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
#include <map> //using instead of vector bc of constant amortized time complexity

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
            return 0U;                                      // if fully-assoc cache
                                                            // address = address << _tagLen;        // this cuts off more significant bits corresponding to tag
        return ((address << _tagLen) >> (32U - _indexLen)); // this cuts off less significant bits corresponding to offset
    }

    uint32_t Cache::getTagFromAddress(uint32_t address) const
    {
        return (address >> (_indexLen + _offsetLen));
    }

    // Destructor for cache
    // need to iterate over cache and delete block's we've created
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
    // upon load hit - increment block lru counters for set w/ hit
    // time = LRU time from block that's hit

    void Cache::loadHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        _loadHits++;
        if (_evictionType == CacheSimulator::LRU)
        {
            for (auto iter = set->begin(); iter != set->end(); iter++)
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
        _cycles++;
    }

    // Check if load hit or load miss given memory address
    void Cache::load(uint32_t index, uint32_t tag)
    {
        // increment loads counter
        _loads++;
        // get index and tag from address

        vector<Block *> *set = nullptr;
        // search cache for block given specified address
        if (sets->find(index) != sets->end())
        {
            set = sets->at(index);
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                if (tag == (*iter)->getTag())
                {
                    // CACHE HIT
                    loadHit(set, (*iter)->getTime());
                    return;
                }
            }
            // CACHE MISS & set exists
            loadMissCase2(set, tag);
        }
        else
        {
            // need to create set
            loadMissCase1(index, tag);
        }
    }

    // create new set
    void Cache::loadMissCase1(uint32_t index, uint32_t tag)
    {
        // create set and add new block to cache
        //    cout << "Entered loadMissCase1" << endl;
        _loadMisses++;
        vector<Block *> *set = new vector<Block *>; // TODO: will need to delete ptr while cleaning up (in destructor?)
        Block *block = new Block(tag);              // TODO: will need to delete ptr while cleaning up (in destructor?)
        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));

        _cycles++;

        // add block to set
        set->push_back(block);
        // add set to map of set-index, set *
        sets->insert({index, set});
    }
    // dont need to create new set
    void Cache::loadMissCase2(vector<Block *> *set, uint32_t tag)
    {
        // case 1: load miss where set already exists
        //  cout << "Entered loadMissCase2" << endl;
        _loadMisses++;

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
                    if (!(_write == CacheSimulator::WRITE_THROUGH && (*iter)->isDirty()))
                    {
                        //write back must always
                        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));
                    }
                    delete *iter; // todo: check all deletes
                    // remove block by erasing item @ position of iterator
                    //instead of removing block - just update values here!
                    set->erase(iter);
                    break;
                }
            }
            _cycles++;
        }
        Block *newBlock = new Block(tag); // TODO: will need to delete ptr while cleaning up (in destructor?)
        // add new block to corresp. set in cache
        set->push_back(newBlock);
        //  cout << "XOXO" << endl;
        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4)) + 1;
    }

    // on store hit: updates blocks and hitBlockTimes, increments cycles according to cache parameters
    void Cache::storeHit(vector<Block *> *set, uint32_t hitBlockTime)
    {
        _storeHits++;
        if (_evictionType == CacheSimulator::LRU)
        {
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                if ((*iter)->getTime() < hitBlockTime)
                {
                    (*iter)->incrementTime();
                }
                else if ((*iter)->getTime() == hitBlockTime)
                {
                    (*iter)->resetTime();
                    if (!(_write == CacheSimulator::WRITE_THROUGH))
                    {
                        (*iter)->setDirty(true);
                    }
                }
            }
        }
        _cycles++;
        if (_write == CacheSimulator::WRITE_THROUGH)
        {
            _cycles += MEM_ACCESS_CYCLES; // TODO: pretty sure this is only + 100
        }
    }

    // Check if store hit or store miss given memory address

    void Cache::store(uint32_t index, uint32_t tag)
    {
        _stores++;
        // get relevant pieces of address

        vector<Block *> *set = nullptr;
        // search the cache for the requested block

        if (sets->find(index) != sets->end())
        {
            set = sets->at(index);
            for (auto iter = set->begin(); iter != set->end(); iter++)
            {
                if (tag == (*iter)->getTag())
                {
                    // STORE HIT

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
                        delete *iter; // todo check all delets
                        iter = set->erase(iter);
                        break;
                    }
                }
            }
            // add the new block
            Block *block = new Block(tag); // TODO: will need to delete ptr while cleaning up (in destructor?)
            set->push_back(block);
            _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));
            if (_write == CacheSimulator::WRITE_THROUGH)
            {
                _cycles += MEM_ACCESS_CYCLES; // TODO: write 4 bytes?
            }
            else
            {
                _cycles++;
                block->setDirty(true);
            }
        }

        // STORE MISS
        else
        {
            _storeMisses++;
            //c1: store miss: NoWriteAlloc

            if (_alloc == CacheSimulator::NO_WRITE_ALLOCATE)
            {
                //only write 4 bytes without bringing entire block into cache
                _cycles += MEM_ACCESS_CYCLES;
                return;
            }
            vector<Block *> *set = new vector<Block *>; // TODO: will need to delete ptr while cleaning up (in destructor?)

            Block *block = new Block(tag);

            //if write allocate: bring block into cache
            _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));

            //c2: store miss: WriteAlloc + WriteThrough

            if (_write == CacheSimulator::WRITE_THROUGH)
            {
                //write 4 bytes to store through to main mem
                _cycles += MEM_ACCESS_CYCLES;
            }
            //c3: store miss: WriteAlloc + WriteBack

            else
            {
                //defer write, transfer 4 bytes into cache
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
