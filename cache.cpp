/* assumptions from assignment description:
 * this is number of cycles to perform
 * load/store operation from specified location
 * to CPU for a single 4 byte quantity
 */
#define CACHE_ACCESS_CYCLES 1U
#define MEM_ACCESS_CYCLES 100U

#define MIN_BLOCK_SIZE 4U

#include "set.h"
#include "block.h"
#include "cache.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <unordered_map> //using instead of vector bc of constant amortized time complexity
#include <cmath>

using std::cout;
using std::endl;
using std::string;
using std::unordered_map;
namespace CacheSimulator
{


    // new functions!
    
    uint32_t Cache::getBlockSize() const
    {
        return _blockSize;
    }

    uint32_t Cache::getNumBlocks() const
    {
        return _numBlocks;
    }
    uint32_t Cache::getBlockIndex(uint32_t index, uint32_t tag)
    {

        Set *set = findSet(index);
        int i = 0;
        for (auto &block : set->_blocks)
        {
            if (block.isValid() && block.getTag() == tag)
            { 
                return i;
            }
            i++;
        }

        return _numBlocks; // 1 beyond max index in block vector
    }

    void Cache::loadToCache(uint32_t index, uint32_t tag)
    {
        cout << "load to cache :)" << endl;
        uint32_t idxToEvict = _numBlocks;

        Set *s = findSet(index);

        if (!s->isFull())
        {
            cout << "set is not full" << endl;

            for (uint32_t i = 0; i < s->_blocks.size(); i++ )
            { // find empty block


                if (!s->_blocks[i].isValid())
                {
                    s->_blocks[i].setValid(true);
                    s->_blocks[i].setTag(tag);
                    s->_blocks[i].setDirty(false);

                this->memoryToCacheOperation();
                s->decrementNumEmptyBlocks();

                    return;
                }
            }
        }
        else
        { // 0 empty _blocks in set, need to evict
                    cout << "set is full: need to evict" << endl;

            idxToEvict = s->findMaxTime();
            if (this->isLRU())
            {
                cout << "inc lru" << endl;
                s->incrementLRU(idxToEvict);
            }
            else
            {
                    cout << "inc fifo" << endl;

                s->incrementFIFO(idxToEvict);
            }

            if (this->isWriteBack())
            {
                cout << "is write back" << endl;
                if (s->_blocks[idxToEvict].isDirty())
                {
                    cout << "block to evict is dirty" << endl;

                    // write modified block back to mem
                    this->memoryToCacheOperation(); 
                }
            } else { //TODO:handle write thru case?


            }
        }
        // update block and "evict old block"
        if (idxToEvict < _numBlocks)
        {
            s->_blocks[idxToEvict].setTag(tag);
            s->_blocks[idxToEvict].setValid(true);
            s->_blocks[idxToEvict].setDirty(false);
        }

        // increment num cycles

        this->memoryToCacheOperation();

    }

    // end of new functions!
    // TODO: need to add to cache.h
    void Cache::addSet(uint32_t index)
    {
        Set s(_numBlocks);
        sets.emplace(index, s);
        _currNumSets++;
    }

    Set *Cache::findSet(uint32_t index)
    {
        auto iter = sets.find(index);
        if (iter != sets.end())
        {
            return &sets[index];
        }
        else
        {
            return new Set(_numBlocks);
        }
    }

    // utilities:
    bool Cache::checkIfCacheHit(uint32_t index, uint32_t tag)
    {

        if (this->findSet(index)->isEmpty() )
        {
            return false;
        }
        else
        {

            if (!this->findSet(index)->findBlockWithTag(tag)->isValid())
            {

                return false;
            }

            return true;
        }
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

    /*    // read:
       void Cache::handleLoadHit(uint32_t index, uint32_t tag)
       {
           // increment load hits, num cycles

           // know set is not empty:
           Set *s = findSet(index);
           Block *b = s->findBlockWithTag(tag);

           if (!b->isValid())
           {
               delete b; // delete dummy block
               handleLoadMiss(index, tag);
           }
           else
           {
               incLoadHits();
               cacheToCpuOperation(); // by 1 for cache access
               // update counters for LRU
               if (isLRU())
                   s->updateLRU(_numBlocks);
           }
       }

       void Cache::handleLoadMiss(uint32_t index, uint32_t tag)
       {
           incLoadMisses();
           Set *s = findSet(index);
           // get corresponding cache block from set

           // case 1: it's empty (i.e. there's no valid data there)
           //+ 100 cycles to load block & + 1 to load block to cache

           // case 2: we have to evict existing block
           // is cache write back?
           // then +100 cycles to write back block
           // mark block as valid, update tag, updateLRU for set
           // mark block as dirty
           // else if write thru:
           //+100 cycles to load block from memory to cache + 1 to load block to cache

           for (auto &block : s->_blocks)
           {
               // iterate thru all blocks in set until you find place to put block

               if (!block.isValid())
               {
                   // update block
                   block.setTag(tag);
                   block.setValid(true);
                   block.resetTime();
                   s->updateLRU(_numBlocks);

                   // increment cycles
                   memoryToCacheOperation();
                   cacheToCpuOperation();

                   return;
               }
           }
           // if you reach here - need to evict block to make room

           int evicted_index = evictBlock(index, tag);
           // if dirty, flip dirty bit and add to cycles bc you

           if (isWriteBack())
               if (evicted_index != -1 && s->_blocks[evicted_index].isDirty())
               {
                   s->findBlockWithTag(tag)->setDirty(false);

                   memoryToCacheOperation(); // load block
               }

           memoryToCacheOperation();

           cacheToCpuOperation();
       }

       // will also set block props = new block props
       int Cache::evictBlock(uint32_t index, uint32_t tag)
       {

           Set *s = findSet(index);
           if (!s->isFull())
               return -1;

           int i = 0;

           for (auto &block : s->_blocks)
           {
               if (block.getTime() >= _numBlocks)
               {
                   return i;
               }
               i++;
           }

           return -1;
       }

       void Cache::handleStoreMiss(uint32_t index, uint32_t tag)
       {
           if (_alloc == WRITE_ALLOCATE)
           {
               writeAllocate(index, tag); // load block from main memory into cache
               memoryToCacheOperation();  // add to cycles in general because you need to load block from main memory

               if (_write == WRITE_THROUGH)
               {
                   writeThrough(index, tag);
                   memoryToCacheOperation();
                   cacheToCpuOperation();
               }
               else
               { // write == WRITE_BACK
                   writeBack(index, tag);
                   cacheToCpuOperation(); // write to cache only
               }
           }
           else
           { // if no-write-allocate just add to cycles
               memoryToCacheOperation();
           }
       }

       void Cache::handleStoreHit(uint32_t index, uint32_t tag)
       {
           if (isWriteThrough())
           {
               writeThrough(index, tag);
               memoryToCacheOperation();
           }
           else
           {                          // it was write-back
               writeBack(index, tag); // writes to cache & sets block=dirty
               // write to cache only
           }
           cacheToCpuOperation();
       }

       void Cache::writeThrough(uint32_t index, uint32_t tag)
       {
           if (isLRU())
           {
               Set *s = findSet(index);

               for (auto &block : s->_blocks)
               { // iterate thru all blocks in set

                   if (block.getTag() == tag && block.isValid())
                   {
                       uint32_t maxTime = block.getTime();
                       block.resetTime();

                       s->updateLRU(maxTime);
                       return;
                   }
               }
           }
       }

       void Cache::writeBack(uint32_t index, uint32_t tag)
       {
           Set *s = findSet(index);
           for (auto &block : s->_blocks)
           { // iterate thru all blocks in set

               if (block.getTag() == tag && block.isValid())
               {
                   block.setDirty(true);
                   if (isLRU())
                   {
                       uint32_t maxTime = block.getTime();
                       block.resetTime();
                       s->updateLRU(maxTime);
                   }
                   return;
               }
           }
       }

       void Cache::writeAllocate(uint32_t index, uint32_t tag)
       {
           // cout << "enter write alloc" << endl;
           Set *s = findSet(index);

           for (uint32_t i = 0; i < _numBlocks; i++)
           {
               Block *b = s->findBlockWithTag(tag);

               if (!b->isValid())
               { // there exists an empty block
                   b->setTag(tag);
                   b->setValid(true);
                   b->resetTime();

                   s->updateLRU(_numBlocks);
                   return;
               }
           }
           // if you get here, need to evict block

           int evicted_index = evictBlock(index, tag);

           if (evicted_index != -1 && findSet(index)->_blocks[evicted_index].isDirty())
           {
               memoryToCacheOperation(); // because you need to write dirty block back to main memory
           }
       } */

    // implement functions: loadToCache
    // incrementFIFO
    // incrementLRU

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
        cout << "incrementing cycles!" << endl;
        _cycles++;
    }

    void Cache::memoryToCacheOperation()
    {
        cout << getBlockSize() << " " << "cycles: " << (100 * (getBlockSize() / 4)) << endl;

        _cycles += (100 * (getBlockSize() / 4));
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