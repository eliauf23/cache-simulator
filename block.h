#ifndef BLOCK_H
#define BLOCK_H

#include <iostream>

namespace CacheSimulator {
class Block
{
public:
    Block();
    Block(bool isDirty, bool isValid, uint32_t tag, uint32_t time);
    Block(uint32_t tag);

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



private:

    bool _isDirty;
    bool _isValid;
    uint32_t _tag; //unique identifier
    uint32_t _time; //counter
};


}

#endif //BLOCK_H