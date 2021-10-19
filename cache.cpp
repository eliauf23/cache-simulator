#include "set.h"
#include "block.h"
#include "cache.h"
#include <string>
#include <sstream>
#include <cstdlib>
#include <map>
#include <unordered_map>

namespace CacheSimulator
{

    //add set to cache as long as you don;t exceed maximum numsets
    Set *Cache::addSet()
    {
        Set *set = new Set(_numBlocks);
        if (_currNumSets + 1 < _numSets)
        {
            sets[_currNumSets++] = *set;
        }
        return set;
    }

    Set *Cache::findSet(uint32_t index)
    {
        std::unordered_map<uint32_t, Set>::iterator iter = sets.find(index);
        if (iter != sets.end())
            return &sets[index];
        Set *set = new Set(_numBlocks);

        return set;
    }

    void Cache::handleStoreMiss(uint32_t address)
    {
        uint32_t index = getIndexFromAddress(address);

        incStoreMisses();

        if (getAlloc() == CacheSimulator::WRITE_ALLOCATE)
        {
            loadFromMainMemory(address);
            incCycles();
            uint32_t cacheHasAddress = find(address);

            if (getWrite() == CacheSimulator::WRITE_BACK)
            {
                //todo: think segfault is happening here
                if (findSet(index)->getBlockAtIndex(cacheHasAddress) != nullptr)
                {
                    findSet(index)->getBlockAtIndex(cacheHasAddress)->setDirty(true);
                }
            }
            else
            {

                addToCycles(100);
            }
        }
        else
        {

            addToCycles(100);
        }
    }

    void Cache::handleStoreHit(uint32_t address)
    {

        uint32_t index = getIndexFromAddress(address);
        //  uint32_t cacheHasAddress = find(address);
        std::cout << "entered handle store hit" << std::endl;

        incStoreHits();

        if (_write == CacheSimulator::WRITE_BACK)
        {
            Set *s = findSet(index);
            
            for (uint32_t i = 0; i < _numBlocks; i++)
            {
                uint32_t thisTag = getTagFromAddress(address);
                if (s->getBlockAtIndex(i)->getTag() == thisTag)
                {
                    s->getBlockAtIndex(i)->setDirty(true);
                }
            }
            incCycles();
        }
        else
        {
            addToCycles(100);
        }
        if (_evictionType == CacheSimulator::LRU)
        {
            Set *s = findSet(index);
            s->evictLRU(index);
        }
    }

    void Cache::handleLoadMiss(uint32_t address)
    {
        incLoadMisses();
        loadFromMainMemory(address);
        incCycles();
    }

    void Cache::handleLoadHit(uint32_t address)
    {
        uint32_t index = getIndexFromAddress(address);
        incLoadHits();
        incCycles();
        if (_evictionType == CacheSimulator::LRU)
        {
            Set *s = findSet(index);
            s->evictLRU(index);
        }
    }

    uint32_t Cache::find(uint32_t address)
    {
        uint32_t index = getIndexFromAddress(address);
        uint32_t tag = getTagFromAddress(address);
        Set *s = findSet(index);
        if (s != nullptr)
        {
            for (uint32_t i = 0U; i < _numBlocks; i++)
            {
                if (s->getBlockAtIndex(i)->isValid() && s->getBlockAtIndex(i)->getTag() == tag)
                {
                    return i;
                }
            }
        }

        return _numBlocks; //index will always be less than associativity
    }

    uint32_t Cache::getIndexFromAddress(uint32_t address) const
    {
        if (_indexLen == 0U)
            return 0U;                                        //if fully-assoc cache
        uint32_t getIndex = (uint32_t)pow(2, _indexLen) - 1U; //shift for zero based indexing
        getIndex = getIndex << _offsetLen;
        uint32_t index = address & getIndex;
        index = index >> _offsetLen;
        return index;
    }
    uint32_t Cache::getTagFromAddress(uint32_t address) const
    {
        std::bitset<32U> bitTag(address);
        std::string tagStr = bitTag.to_string().substr(0, _tagLen);
        uint32_t tag;
        std::stringstream tag_stream(tagStr);
        tag_stream >> std::dec >> tag;
        return tag;
    }
    uint32_t Cache::getOffsetFromAddress(uint32_t address) const
    {
        //TODO: implement if nec
        return 0U;
    }

    void Cache::loadFromMainMemory(uint32_t address)
    {

        _cycles += (_blockSize / 4) * 100;

        uint32_t index = getIndexFromAddress(address);
        Set *s = findSet(index);
        if (s->isEmpty())
        {

            bool found = false;
            for (uint32_t i = 0; i < s->getNumBlocks() && !found; i++)
            {
                if (!s->getBlockAtIndex(i)->isValid())
                {
                    //set block;
                    s->getBlockAtIndex(i)->setValid(true);
                    s->getBlockAtIndex(i)->setDirty(false);
                    s->getBlockAtIndex(i)->setTag(address >> (_indexLen + _offsetLen));
                    s->getBlockAtIndex(i)->setTime(0);
                    found = true;
                }
            }
        }
        else
        {

            //get index to evict - i.e. max time?
            uint32_t indexToEvict = 0U;
            uint32_t maxTime = 0U;
            for (uint32_t j = 0; j < s->getNumBlocks(); j++)
            {
                uint32_t blockTime = s->getBlockAtIndex(j)->getTime();
                if (blockTime >= maxTime)
                {
                    maxTime = blockTime;
                    indexToEvict = j;
                }
            }

            if (_evictionType == CacheSimulator::LRU)
            {
                s->evictLRU(indexToEvict);
            }
            else if (_evictionType == CacheSimulator::FIFO)
            {
                s->evictFIFO(indexToEvict);
            }
        }
    }

    void Cache::printResults()
    {
        //prints out results with required formatting

        std::cout << "Total loads: " << _loadMisses + _loadHits << std::endl;
        std::cout << "Total stores: " << _storeMisses + _storeHits << std::endl;
        std::cout << "Load hits: " << _loadHits << std::endl;
        std::cout << "Load misses: " << _loadMisses << std::endl;
        std::cout << "Store hits: " << _storeHits << std::endl;
        std::cout << "Store misses: " << _storeMisses << std::endl;
        std::cout << "Total cycles: " << _cycles << std::endl;
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
    void Cache::addToCycles(uint32_t n)
    {
        _cycles += n;
    }

}