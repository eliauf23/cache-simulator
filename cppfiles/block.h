#ifndef __BLOCK__H__
#define __BLOCK__H__

#include <iostream>

using namespace std;

class Block {
    public:
        //public functions/props declarations
    private:
    uint32_t address;
    uint32_t lastAccessTime;
    bool isDirty;

    
        //helper function/props declarations

    Block() {
    }; //constructor
};

Block::Block() {
    
}


#endif  //!__BLOCK__H__