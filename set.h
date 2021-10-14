#ifndef __SET__H__
#define __SET__H__


#include "block.h"

#include <iostream>
#include <vector>

using namespace std::vector;
using namespace Block;

namespace Set {
    
class Set {
    private:
    std::vector <Block> blocks; //array of blocks
    //blockTag - string?

    bool hasData = false;



    public:

    //constructor to create empty set
    Set() {
        hasData = true;
    };

    bool hasData() {
        return hasData;
    }

    unsigned int getSize() {
        return blocks.size();
    }

    Block findBlock(std::string tag) {
        //find block will return null value if DNE
    }
    bool writeBlock(uint32_t address) {
        //return true if write is sucessful
        //false otherwise
        
    }


}
}


#endif //!__SET__H__