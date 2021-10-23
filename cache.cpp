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
#include <map>

#include <cmath>

using std::cout;
using std::endl;
using std::string;

namespace CacheSimulator
{

    //utilities:
    bool Cache::checkIfCacheHit(uint32_t index, uint32_t tag)
    {

        Set s = _sets.at(index);

        for (uint32_t i = 0; i < _numBlocks; i++)
        {
            Block b = s._blocks.at(i);

            // cout << "Block tag" << b.getTag()<< endl;
            // cout << "This tag" << tag << endl;

            if (b.isValid() && b.getTag() == tag)
            {

                return true;
            }
        }

        return false;
    }


    uint32_t Cache::getIndexFromAddress(uint32_t address) const
    {



        if (_indexLen == 0U) return 0U; // if fully-assoc cache
        address = address << _tagLen;
        return address >> (32U - _indexLen);


        // std::bitset<32U> bitIndex(address);
        // string idxStr = bitIndex.to_string().substr(32 - _offsetLen - _indexLen, _indexLen);
        // uint32_t index = (uint32_t)std::stoul(idxStr, 0, 2);
        // cout << index << endl;

        // return index;
    }

    uint32_t Cache::getTagFromAddress(uint32_t address) const
    {
        return address >> (_indexLen + _offsetLen);
        // std::bitset<32U> bitTag(address);

        // string tagStr = bitTag.to_string().substr(0, _tagLen);
        // cout << tagStr << endl;
        // uint32_t tag = (uint32_t)std::stoul(tagStr, 0, 2);
        // cout << tag << endl;

        // return tag;
    }


    //read:
    void Cache::handleLoadHit(uint32_t index, uint32_t tag)
    {
        incCycles();

        if (_evictionType == CacheSimulator::LRU)
        {

            Set s = _sets.at(index);
            for (uint32_t i = 0; i < _numBlocks; i++)
            {
                Block b = s._blocks.at(i);

                if (b.getTag() == tag && b.isValid())
                {
                    uint32_t targetBlockTimestamp = b.getTime();
                    b.resetTime();
                    updateLRU(index, tag, targetBlockTimestamp);
                    _sets[index]._blocks[i] = b;
                }
            }
        }
    }

    void Cache::handleLoadMiss(uint32_t index, uint32_t tag)
    {



        Set s = _sets.at(index);

        for (uint32_t i = 0; i < _numBlocks; i++)
        { // iterate thru all blocks in set

            Block b = s._blocks.at(i);
            if (!b.isValid())
            {
                b.setTag(tag);
                b.setValid(true);
                b.resetTime();
                _sets[index]._blocks[i] = b;
                updateLRU(index, tag, _numBlocks);
                addToCycles();
                incCycles();
                return;
            }
        }
  
  
        int evicted_index = getIndexToEvict(index, tag);

        if (evicted_index != -1 && s._blocks.at(evicted_index).isDirty())
        {

            s._blocks.at(evicted_index).setDirty(false);
            addToCycles();
        }
        addToCycles();
        incCycles();
}

    int Cache::getIndexToEvict(uint32_t index, uint32_t tag)
    {

        Set s = _sets.at(index);

        for (uint32_t i = 0; i < _numBlocks; i++)
        {

            Block b = s._blocks.at(i);

            if (b.getTime() == _numBlocks)
            {
                b.setTag(tag);
                b.setValid(true);
                b.resetTime();
                updateLRU(index, tag, _numBlocks);
                return i;
            }
        }

        return -1;
    }

    void Cache::updateLRU(uint32_t index, uint32_t tag, uint32_t maxTime)
    {
        for (uint32_t j = 0; j < _numBlocks; j++)
        {
            if ((_sets.at(index)._blocks.at(j).getTag() != tag) && _sets.at(index)._blocks.at(j).isValid() && _sets.at(index)._blocks.at(j).getTime() < maxTime)
            {
                _sets.at(index)._blocks.at(j).incrementTime();
            }
        }
    }


    void Cache::handleStoreMiss(uint32_t index, uint32_t tag)
    {
        if (_alloc == WRITE_ALLOCATE)
        {
            writeAllocate(index, tag); // load block from main memory into cache
           
           
            if (_write == WRITE_THROUGH)
            {
                writeThrough(index, tag);
            }
            else
            { // write == WRITE_BACK
                writeBack(index, tag);
            }
        }
        else
        { // if no-write-allocate just add to cycles
            addToCycles();
        }
    }

    void Cache::handleStoreHit(uint32_t index, uint32_t tag)
    {
        if (_write == CacheSimulator::WRITE_THROUGH)
        {
            writeThrough(index, tag); // writes to cache & main mem
        }
        else
        {                          // it was write-back
            writeBack(index, tag); // writes to cache & sets block=dirty
        }
    }

    void Cache::writeThrough(uint32_t index, uint32_t tag)
    {
        if (_evictionType == CacheSimulator::LRU)
        {
            Set s = _sets.at(index);

            for (uint32_t i = 0; i < _numBlocks; i++)
            { // iterate thru all blocks in set
                Block b = s._blocks.at(i);
                if (b.getTag() == tag && b.isValid())
                {
                    uint32_t maxTime = b.getTime();
                    b.resetTime();
                    _sets[index]._blocks[i] = b;

                    updateLRU(index, tag, maxTime);
                    addToCycles();
                    incCycles();
                    return;
                }
            }
        }

        addToCycles();
        incCycles();
    }

    void Cache::writeBack(uint32_t index, uint32_t tag)
    {
        Set s = _sets.at(index);
        for (uint32_t i = 0; i < _numBlocks; i++)
        { // iterate thru all blocks in set
            Block b = s._blocks.at(i);
            if (b.getTag() == tag && b.isValid())
            {                  // if block in question
                b.setDirty(1); // mark dirty bit
                if (_evictionType == CacheSimulator::LRU)
                {
                    uint32_t oldOrder = b.getTime();
                    b.resetTime();
                    _sets[index]._blocks[i] = b;
                    updateLRU(index, tag, oldOrder);
                }
                break;
            }
        }

        incCycles(); //write to cache only
    }

    void Cache::writeAllocate(uint32_t index, uint32_t tag)
    {
        //cout << "enter write alloc" << endl;
        Set s = _sets.at(index);
        bool needToEvict = true;
        for (uint32_t i = 0; i < _numBlocks; i++)
        {
            Block b = s._blocks.at(i);
            if (!b.isValid())
            {
                b.setTag(tag);
                b.setValid(true);
                b.resetTime();
                needToEvict = false;
                _sets[index]._blocks[i] = b;
                updateLRU(index, tag, _numBlocks);
        addToCycles(); //add to cycles in general because you need to load block from main memory
           return;
            }
        }
        if (needToEvict)
        {
            int evicted_index = getIndexToEvict(index, tag);

            if (evicted_index != -1 && _sets.at(index)._blocks[evicted_index].isDirty())
            {
                addToCycles(); //because you need to write dirty block back to main memory
            }
        }

        addToCycles(); //add to cycles in general because you need to load block from main memory
    }


    void Cache::printResults()
    {
        // prints out results with required formatting

        cout << "Total loads: " << _loadMisses + _loadHits << endl;
        cout << "Total stores: " << _storeMisses + _storeHits << endl;
        cout << "Load hits: " << _loadHits << endl;
        cout << "Load misses: " << _loadMisses << endl;
        cout << "Store hits: " << _storeHits << endl;
        cout << "Store misses: " << _storeMisses << endl;
        cout << "Total cycles: " << _cycles << endl;
    }

    Allocation Cache::getAlloc() const
    {
        return _alloc;
    }

    void Cache::setAlloc(Allocation alloc)
    {
        _alloc = alloc;
    }

    Write Cache::getWrite() const
    {
        return _write;
    }

    void Cache::setWrite(Write write)
    {
        _write = write;
    }

    uint32_t Cache::getNumBlocks() const
    {
        return _numBlocks;
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

    void Cache::incCycles()
    {
        _cycles++;
    }

    void Cache::addToCycles()
    {
        _cycles += (MEM_ACCESS_CYCLES * (_blockSize / 4));
    }
}