#include "set.h"
#include "block.h"
#include "cache.h"
#include <string>
#include <cstdlib>

namespace CacheSimulator
{

    Set *addSet(std::string index)
    {
        Set *set = new Set();
        sets[index] = *set;
        _currNumSets++;
        return set;
    }

    Set *findSet(std::string index)
    {
        std::unordered_map<std::string, Set>::iterator iter = sets.find(index);

        if (iter != sets.end())
            return &sets[index];

        Set *set = new Set(true);
        return set;
    }

    void readFromCache(Block *block, Set *set, std::string tag)
    {
        if (block->isEmpty())
        {
            delete block;

            _loadMisses++;
            
            _cycles += 100 * _blockSize / 4 + 1;

            if (set->getNumBlocksStored() == _numBlocks)
            {
                block = set.findBlockFromTime(_numBlocks - 1);
                if (_write == WRITE_BACK && block->isDirty())
                {
                    _cycles += 100 * _blockSize / 4;
                    block->resetDirty();
                }
                set.countTimerAll();
                block.resetTime();
                block.updateTag(tag);
            }
            else
            {
                set.countTimerAll();
                block = new Block(tag);
                set.addBlock(*block);
                delete block;
            }
        }
        else
        {
            _loadHits++;
            _cycles++;
            if (_evictionType == LRU)
                set->incTime(block->getLastAccessTime());
        }
    }

    int read(std::string index, std::string tag, std::string &firstTag)
    {
        Set *set = findSet(index);
        Block *block;

        if (set.isEmpty())
        {
            delete set;
            if (getNumSets() == _numSets)
            {
                std::cerr << "Index is invalid" << std::endl;
                return -1;
            }
            _loadMisses++;

            set = addSet(index);
            block = new Block(tag);
            set.addBlock(*block);
            firstTag = tag;

            _cycles += 100 * _blockSize / 4 + 1;

            delete set;
            delete block;
        }
        else
        {
            block = set.findBlock(tag);
            readFromCache(block, set, tag);
        }
        set = NULL;
        block = NULL;

        return 0;
    }

    int write(std::string index, std::string tag, std::string &firstTag, bool &firstDirty)
    {
        Set *set = findSet(index);
        Block *block = nullptr;

        if (set.isEmpty())
        {
            delete set;
            _storeMisses++;

            if (getNumSets() == _numSets)
            {
                std::cerr << "Invalid index" << std::endl;
                return -1;
            }

            writeCache(set, block, firstTag, firstDirty, index, tag);
        }
        else
        {
            writeIfSetNotEmpty(set, block, tag);
        }
        set = nullptr;
        block = nullptr;

        return 0;
    }

    void writeCache(Set *set, Block *block, std::string &firstTag, bool &firstDirty, std::string index, std::string tag)
    {
        if (_allocPolicy == WRITE_ALLOCATE)
        {
            set = addSet(index);
            block = new Block(tag);
            set.addBlock(*block);

            firstTag = tag;
            firstDirty = true;

            delete set;
            delete block;

            _cycles += 100 * _blockSize / 4;

            if (_write == WRITE_THROUGH)
                _cycles += 101;
            else
                _cycles++;
        }
        else
            _cycles += 100;
    }

    void writeIfSetNotEmpty(Set *set, Block *block, std::string tag)
    {
        block = set.findBlock(tag);

        if (block.isEmpty())
        {
            writeToCache(set, block, tag);
        }
        else
        {
            _storeHits++;

            if (_write == WRITE_BACK)
                block.setDirty();
            if (_allocPolicy == NO_WRITE_ALLOCATE)
                _cycles += 100;
            else if (_write == WRITE_THROUGH)
                _cycles += 101;
            else
                _cycles++;

            if (_evictionType == LRU)
                set.countTimer(block.getTime());
        }
    }

    void writeToCache(Set *set, Block *block, std::string tag)
    {
        delete block;
        _storeMisses++;

        if (_alloc == WRITE_ALLOCATE)
        {
            updateBlock(set, block, tag);

            if (_allocPolicy == WRITE_ALLOCATE)
            {
                _cycles += 100 * _blockSize / 4;

                if (_write == WRITE_THROUGH)
                    _cycles += 101;
                else
                    _cycles++;
            }
        }
        else
            _cycles += 100;
    }

    void updateBlock(Set *set, Block *block, std::string tag)
    {
        if (_numBlocks == set.getNumBlocksStored())
        {
            block = set.findBlockFromTime(_numBlocks - 1);

            if (_write == WRITE_BACK && block->isDirty())
                _cycles += 100 * _blockSize / 4;

            set.countTimerAll();
            block.resetTime();
            block.updateTag(tag);
        }
        else
        {
            set.countTimerAll();
            block = new Block(tag);
            set.addBlock(*block);

            if (_write == WRITE_BACK)
                block.setDirty();

            delete block;
        }
    }
}