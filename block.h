#ifndef __BLOCK__H__
#define __BLOCK__H__

#include <iostream>

class Block
{
public:
    //TODO: create getters & setters for all fields (after we see what we need)
    Block()
    { //default - empty and unmodified
        _isDirty = false;
        _isEmpty = true;
        _tag = "";
        _timeCreated = 0;
        _lastAccessTime = 0;
    };

    Block(std::string tag)
    { //default - empty and unmodified
        _tag = "";
        _isDirty = false;
        _isEmpty = true;
        _timeCreated = 0;
        _lastAccessTime = 0;
    };

    //public methods:

    //modify fields
    void incLastAccessTime();



    //clear fields
    void resetTimeCreated();
    void resetDirty();


    //setters
    void setDirty();
    void setEmpty();
    void setTag(std::string newTag);

    //getters

    int getLastAccessTime()
    {
        return _lastAccessTime;
    }
    int getTimeCreated()
    {
        return _timeCreated;
    }
    std::string getTag()
    {
        return _tag;
    }

    bool isDirty()
    {
        return _isDirty;
    }

    bool isEmpty()
    {
        return _isEmpty;
    }

private:
    //fields
    bool _isDirty;
    bool _isEmpty;
    std::string _tag;
    uint32_t _timeCreated;
    uint32_t _lastAccessTime;
};

#endif //!__BLOCK__H__