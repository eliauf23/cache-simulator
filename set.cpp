#include "set.h"
#include "block.h"

namespace CacheSimulator
{
    Set::Set() {}

    Set::Set(uint32_t numBlocks)
    {

        _numBlocks = numBlocks;
        _emptyBlocks = numBlocks;

        for (uint32_t i = 0; i < numBlocks; i++)
        {
            _blocks.emplace_back(Block());
        }
    }

    void Set::decrementNumEmptyBlocks()
    {
        _emptyBlocks--;
    }

    uint32_t Set::getNumEmptyBlocks()
    {
        return _emptyBlocks;
    }
    Block *Set::findBlockWithTag(uint32_t tag)
    {
        for (auto &_block : _blocks)
        {
            if (_block.getTag() == tag)
                return &_block;
        }
        return new Block();
    }

    bool Set::isEmpty() const
    {
        for (auto &_block : _blocks)
        {
            if (_block.isValid())
                return false;
        }
        return true;
    }
    bool Set::isFull() const
    {
        uint32_t counter = 0;
        for (auto &_block : _blocks)
        {
            if (_block.isValid())
                counter++;
        }
        return counter == _numBlocks;

        // TODO: change to empty blocks == 0
    }

    Set::~Set() = default;

    uint32_t Set::findMaxTime()
    {

        uint32_t maxTime = 0;
        uint32_t maxIdx = 0;
        uint32_t i = 0;
        for (auto &block : _blocks)
        {
            {
                if (block.getTime() > maxTime)
                {
                    maxTime = block.getTime();
                    maxIdx = i;
                }
                i++;
            }
        }
        return maxIdx;
    }

    void Set::incrementLRU(uint32_t idxToAccess)
    {
        uint32_t accessCounter = _blocks[idxToAccess].getTime();
        if (accessCounter == 0)
            return;

        for (auto &block : _blocks)
        {

            if (block.isValid() && block.getTime() <= accessCounter)
            {
                block.incrementTime();
            }
        }

        _blocks[idxToAccess].resetTime();
    }

    void Set::incrementFIFO(uint32_t idxToAccess)
    {
        for (auto &block : _blocks)
        {
            block.incrementTime();
        }

        _blocks[idxToAccess].resetTime();
    }
}