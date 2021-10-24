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
        Set(uint32_t numBlocks);

        Set();

        // add destructor?

        uint32_t findMaxTime();
        void incrementLRU(uint32_t idxToAccess);
        void incrementFIFO(uint32_t idxToAccess);
        void decrementNumEmptyBlocks();
        uint32_t getNumEmptyBlocks();

        Block *findBlockWithTag(uint32_t tag);
        bool isEmpty() const;
        void updateLRU(uint32_t maxTime);
        bool isFull() const;
        virtual ~Set();

        std::vector<Block> _blocks;

    private:
        uint32_t _numBlocks;
        uint32_t _emptyBlocks;
    };
}

#endif // SET_H