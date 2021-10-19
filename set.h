#ifndef SET_H
#define SET_H

#include "block.h"

#include <iostream>
#include <vector>

namespace CacheSimulator
{

    class Set {
    public:
        Set() {
            _numBlocks = 0;
            _emptyBlocks = 0;
        };
        Set(uint32_t numBlocks);

        void addBlock(Block block);
        void evictLRU(uint32_t i);
        void evictFIFO(uint32_t i);

        Block* getBlockAtIndex(uint32_t i);
        bool isEmpty();
        uint32_t getNumBlocks() const;

        void setNumBlocks(uint32_t numBlocks);

        uint32_t getEmptyBlocks() const;

        void setEmptyBlocks(uint32_t emptyBlocks);

    private:
        std::vector <Block> _blocks;
        uint32_t _emptyBlocks;
        uint32_t _numBlocks;


    };
}

#endif //SET_H