#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

namespace CacheSimulator
{
    class Block
    {
    public:
        Block();
        ~Block() {}

        Block(bool isDirty, bool isValid, uint32_t tag, uint32_t time); // constructor

        Block(uint32_t tag); //

        /*
         * Checks the _isDirty flag on the block. Returns true if the block is dirty
         * and false if it is clean.
         *
         * Parameters:
         *
         * Returns:
         *   true -  if the block is dirty
         *   false - if the block is clean.
         */
        bool isDirty() const;

        /*
         * Sets _isDirty parameter for the block as true
         *
         *
         * Parameters:
         *   isDirty - boolean flag for dirty bit in block
         *
         * Returns:
         *   void
         */
        void setDirty(bool isDirty);

        /*
         * Checks the valid flad on the block which
         * represents if data has been written to the block. Returns true
         * if data is written to the block and false if its an empty block
         * with no data
         *
         * Patameters:
         *
         * Returns:
         *   true - if valid data is written to the block
         *   false -  if an empty block with no data
         */
        bool isValid() const;

        /*
         * Checks to see if the valid tag is set to true
         *
         * Parameters:
         *   isValid - boolean tag from block
         *
         * Returns:
         *   void
         */
        void setValid(bool isValid);

        /*
         * returns the tag, which is the unique identifier, of the block
         *
         * Parameters:
         *
         * Returns:
         *   _tag variable of the block
         */
        uint32_t getTag() const;

        /*
         * sets the tag variable of a block to the new given value
         *
         * Parameter:
         *   tag - tag value to be updated
         *
         * Retuns:
         *   void
         */
        void setTag(uint32_t tag);

        /*
         * gets the _time variable for eviction purposes
         *
         * Parameter:
         *
         * Returns:
         *   _time - for FIFO, this is the order in which it was added and for LRU it is the
         *           time count for how recently its been used
         */
        uint32_t getTime() const;

        /*
         * setter function for _time variable for eviction purposes
         *
         * Parameter:
         *   time - time value with which to update var
         *
         * Returns:
         */
        void setTime(uint32_t time);

        /*
         *  increments time variable of a block by one
         *
         * Parameter:
         *
         * Returns:
         */
        void incrementTime();

        /*
         * Sets time variable to 0. Used when block is evicted and rewritten
         *
         * Parameters:
         *
         * Returns:
         *
         */
        void resetTime();

    private:
        bool _isDirty;  // property to store dirty/clean block
        bool _isValid;  // property to store valid/invalid status
        uint32_t _tag;  // unique identifier
        uint32_t _time; // eviction counter
    };

}

#endif // BLOCK_H