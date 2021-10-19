#include "set.h"
#include "block.h"

namespace CacheSimulator
{

    void Set::addBlock(Block block)
    {
        _blocks.push_back(block);
        if (block.isValid())
        {
            _emptyBlocks--;
        }
    }

    void Set::evictLRU(uint32_t evictIdx)
    {

        uint32_t lastAccessTime = getBlockAtIndex(evictIdx)->getTime();
        if (lastAccessTime == 0)
        {
            return;
        }

        for (uint32_t i = 0; i < _numBlocks; i++)
        {
            Block *b = getBlockAtIndex(i);

            if (b != nullptr)
            {
                if (b->isValid() && b->getTime() <= lastAccessTime)
                {
                    b->incrementTime();
                }
            }
        }
        getBlockAtIndex(evictIdx)->setTime(0);
    }
    void Set::evictFIFO(uint32_t evictIdx)
    {
        for (uint32_t i = 0; i < _numBlocks; i++)
        {
            getBlockAtIndex(i)->incrementTime();
        }
        _blocks[evictIdx].resetTime();
    }

    //generates empty set
    Set::Set(uint32_t numBlocks) :  _emptyBlocks(numBlocks) ,_numBlocks(numBlocks){}

    Block *Set::getBlockAtIndex(uint32_t i)
    {
        if (i >= 0 && i <= _numBlocks - 1)
        {
            return &_blocks[i];
        }
        else
        {
            return nullptr;
        }
    }

    bool Set::isEmpty()
    {
        return (_numBlocks == 0);
    }

    uint32_t Set::getNumBlocks() const
    {
        return _numBlocks;
    }

    void Set::setNumBlocks(uint32_t numBlocks)
    {
        _numBlocks = numBlocks;
    }

    uint32_t Set::getEmptyBlocks() const
    {
        return _emptyBlocks;
    }

    void Set::setEmptyBlocks(uint32_t emptyBlocks)
    {
        _emptyBlocks = emptyBlocks;
    }

}