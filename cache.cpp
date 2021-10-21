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

#include <bitset>
#include <cmath>
namespace CacheSimulator
{

    //add set to cache as long as you don;t exceed maximum numsets
    // bool Cache::isHit() {
    //     if(_numBlocks == 1) { //numBlocks per set = 1 -> direct map
    //         //use address: 
    //         //get index
    //         //use index to get the set
    //         /
    //                 }            return(getBlockAtIndex(0) == )ock[0] has tag = thisTag

    //         //fully associative: 1 set, 
    //         //get set[0] & 
    //     }
    // }


    Set *Cache::addSet(uint32_t index)
    {

        Set *set = new Set(index);
        _sets.push_back(set);
        _currNumSets++;

        return set;
    }

    Set *Cache::findSet(uint32_t index)
    {

    std::cout << "entered find set w/ index = " << index << std::endl; 

       Set * set= _sets[index];

        if(set== nullptr) set = new Set(_numBlocks);
        return set;
    }

 void Cache::handleLoadMiss(uint32_t address)
    {
        incLoadMisses();
        loadFromMainMemory(address);
        incCycles();
    }

    void Cache::handleLoadHit(uint32_t address)
    {
        incLoadHits(); //load hit determined by main
        incCycles(); //+1 cycle for a cache load
        if (_blockSize == 1) {return;}    //If direct mapped (aka block size == 1), then update counters and exit
        uint32_t index = getIndexFromAddress(address);
        Set *s = findSet(index);
        

        //If either set or full associative, use LRU or FIFO to update cache 
        if (_evictionType == CacheSimulator::LRU && _blockSize != 1) //TODO: Check that second arg is needed
        {
            s->evictLRU(index); //Here evict isn't actually evicting, its updating LRU counter
        }
        else if (_evictionType == CacheSimulator::FIFO && _blockSize != 1) 
        {
            s->evictFIFO(index); //evict isn't actually evicting, its updating FIFO counter
        }
        delete s;
    }


    void Cache::handleStoreMiss(uint32_t address)
    {
        uint32_t index = getIndexFromAddress(address);
        Set *s = findSet(index);

        incStoreMisses();

        if (getAlloc() == CacheSimulator::WRITE_ALLOCATE)
        {
            //for write-allocate we bring the relevant memory block into the cache before the store proceeds
            loadFromMainMemory(address);
            addToCycles(); //bc we load from memory we need to add 100* # of 4 bytes to cycle counter
            incCycles(); // add one cycle for store
            uint32_t cacheHasAddress = find(address);

            if (getWrite() == CacheSimulator::WRITE_BACK)
            {
                //store writes to the cache only and marks the block dirty
                //if the block is evicted later, it has to be written back to memory before being replaced
                //todo: think segfault is happening here
                if (s->getBlockAtIndex(cacheHasAddress) != nullptr)
                {
                    s->getBlockAtIndex(cacheHasAddress)->setDirty(true);
                }
            }
            else
            {
                addToCycles();
            }
        }
        else
        {
            addToCycles();
        }
        delete s;
    }

    void Cache::handleStoreHit(uint32_t address)
    {

        uint32_t index = getIndexFromAddress(address);
        //  uint32_t cacheHasAddress = find(address);
        std::cout << "entered handle store hit" << std::endl;

        incStoreHits();
        Set *s = findSet(index);

        if (_write == CacheSimulator::WRITE_BACK)
        {
            
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
            addToCycles();
        }
        if (_evictionType == CacheSimulator::LRU)
        {

            s->evictLRU(index);
        }    
        delete s;
        }

   

//check for hit
    uint32_t Cache::find(uint32_t address)
    //returns block # of address in the set 
    {
        uint32_t index = getIndexFromAddress(address);
        uint32_t tag = getTagFromAddress(address);
        Set *s = findSet(index);
        if (s->getNumBlocks() != _numBlocks)
        {


            for (uint32_t i = 0; i < _numBlocks; i++)
            {

                if (s->getBlockAtIndex(i)->isValid() && s->getBlockAtIndex(i)->getTag() == tag)
                {
                    delete s;
                    return i;
                } else {
                    s->getBlockAtIndex(i)->setValid(true);
                    s->getBlockAtIndex(i)->setTag(tag);
                }
            }

        }


        delete s;

        return _numBlocks; //index will always be less than associativity
    }

    uint32_t Cache::getIndexFromAddress(uint32_t address) const
    {
        if (_indexLen == 0U) return 0U; //if fully-assoc cache
       
        std::bitset<32U> bitIndex(address);
        std::string idxStr = bitIndex.to_string().substr(_offsetLen, _offsetLen+_indexLen);
        uint32_t index;
        std::stringstream tag_stream(idxStr);
        tag_stream >> std::dec >> index;
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

        addToCycles();
        uint32_t index = getIndexFromAddress(address);
        Set *s = findSet(index);
       //checking if not directly mapped & the set isn't empty 
        if (!s->isEmpty() && _numBlocks != 1U)
        {

            Block *b;
            bool found = false;
            for (uint32_t i = 0; i < s->getNumBlocks() && !found; i++)
            {
                            b = s->getBlockAtIndex(i);

                //TODO: look at this!
                if (!b->isValid())
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
            Block *b;
            for (uint32_t j = 0; j < s->getNumBlocks(); j++)
            {
                b = s->getBlockAtIndex(j);
                uint32_t blockTime = b->getTime();
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
            delete b;
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

    //
    void Cache::addToCycles()
    {
        _cycles += (MEM_ACCESS_CYCLES * _blockSize / 4);
    }


}