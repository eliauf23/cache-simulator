#include "block.h"


namespace CacheSimulator {


    void Block::setDirty() {
        _isDirty = true;
    }
    void Block::resetDirty() {
        _isDirty = false;
    }

    void Block::incLastAccessTime() {
        _lastAccessTime++;
    }

    void Block::resetTimeCreated() {
        _timeCreated = 0;
    }

    void Block::setTag(std::string tag) {
        _tag = std::move(tag);
    }

    void Block::setEmpty() {
        _isEmpty = true;
    }
}