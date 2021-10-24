#include "set.h"
#include "block.h"

namespace CacheSimulator
{
    Set::Set() {}

    Set::Set(uint32_t numBlocks)
    {

        _numBlocks = numBlocks;
        for (uint32_t i = 0; i < numBlocks; i++)
        {
            _blocks.emplace_back(Block());
        }
    }
    Block * Set::findBlockWithTag(uint32_t tag) {
        for (auto & _block : _blocks) {
            if (_block.getTag() == tag)
                return &_block;
        }
        return new Block();
    }

    bool Set::isEmpty() const {
        for (auto & _block : _blocks) {
            if (_block.isValid()) return false;
        }
        return true;

    }
    bool Set::isFull() const {
        uint32_t counter = 0;
        for (auto & _block : _blocks) {
            if (_block.isValid()) counter++;
        }
        return counter == _numBlocks;

    }


    Set::~Set() = default;
    void Set::updateLRU(uint32_t maxTime)
    {
        for (auto & block : _blocks) {
            if(block.getTime() < maxTime) {
                block.incrementTime();
            } else {
                block.resetTime();
            }
        }
    }


}