#include "block.h"

namespace CacheSimulator {
    Block::Block() {
        _isDirty = false;
        _isValid = false;
        _tag = 0;
        _time = 0;
    }

    Block::Block(bool isDirty, bool isValid, uint32_t tag, uint32_t time) : _isDirty(isDirty), _isValid(isValid),
                                                                            _tag(tag), _time(time) {}
    Block::Block(uint32_t tag) : _isDirty(false), _isValid(false), _tag(tag), _time(0) {}
    
    bool Block::isDirty() const {
        return _isDirty;
    }

    void Block::setDirty(bool isDirty) {
        _isDirty = isDirty;
    }

    bool Block::isValid() const {
        return _isValid;
    }

    void Block::setValid(bool isValid) {
        _isValid = isValid;
    }

    uint32_t Block::getTag() const {
        return _tag;
    }

    void Block::setTag(uint32_t tag) {
        _tag = tag;
    }

    uint32_t Block::getTime() const {
        return _time;
    }

    void Block::setTime(uint32_t time) {
        _time = time;
    }


    void Block::incrementTime() {
        _time++;
    }

    void Block::resetTime() {
        _time = 1;
    }
}