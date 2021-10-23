#ifndef SET_H
#define SET_H

#include "block.h"

#include <iostream>
#include <vector>

namespace CacheSimulator
{

    class Set
    {
    public:
        Set(uint32_t numBlocks, uint32_t index);
        // add destructor?

        std::vector<Block> _blocks;

    private:
        uint32_t _setIndex;
        uint32_t _emptyBlocks;
        uint32_t _numBlocks;
    };
}

#endif // SET_H