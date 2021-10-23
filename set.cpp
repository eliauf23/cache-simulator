#include "set.h"
#include "block.h"

namespace CacheSimulator
{

    Set::Set(uint32_t numBlocks, uint32_t index)
    {

        _numBlocks = numBlocks;
        _emptyBlocks = numBlocks;
        _setIndex = index;
        for (uint32_t i = 0; i < numBlocks; i++)
        {
            _blocks.push_back(Block());
        }
    }

}