#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

namespace CacheSimulator {
class Block
{
public:
    Block(bool isDirty, bool isValid, uint32_t tag, uint32_t time);

    bool isDirty() const;

    void setDirty(bool isDirty);

    bool isValid() const;

    void setValid(bool isValid);

    uint32_t getTag() const;

    void setTag(uint32_t tag);

    uint32_t getTime() const;

    void setTime(uint32_t time);

    void incrementTime();

    void resetTime();

//TODO: make these fields private
    bool _isValid;
    uint32_t _tag; //identifier
private:

    bool _isDirty;
    uint32_t _time; //counter
};


}

#endif //BLOCK_H