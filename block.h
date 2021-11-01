#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

namespace CacheSimulator {
class Block
{
public:
    Block();
    ~Block() {}

    Block(bool isDirty, bool isValid, uint32_t tag, uint32_t time); //constructor
    
    Block(uint32_t tag); //

    
    bool isDirty() const;
    //Returns true if block is dirty
    //returns false if block is clean



    void setDirty(bool isDirty);
    //sets block to dirty. (isDirty() will now return true)

    bool isValid() const;
    //returns true if a value has been stored to the block 
    //returns false if empty

    void setValid(bool isValid);
    //sets _isValid property of block to valid
    

    uint32_t getTag() const;
    //returns _tag of block. 
    //tag is unique identifier of block

    void setTag(uint32_t tag);
    //sets _tag of block. Essentially loading from memory

    uint32_t getTime() const;
    //returns _time property for eviction purposes

    void setTime(uint32_t time);
    //sets _time property 

    void incrementTime();
    //increments _time property by 1

    void resetTime();
    //sets _time to 0. Used when block is evicted and rewritten



private:

    bool _isDirty; //property to store dirty/clean block
    bool _isValid; //property to store valid/invalid status
    uint32_t _tag; //unique identifier
    uint32_t _time; //eviction counter
};


}

#endif //BLOCK_H