#include "csim_fns.h"



namespace CacheSimulator
{
    //returns 1 if number is a power of 2, 0 if not a power of 2

unsigned int isPowerOfTwo(uint32_t num) {
            return (num && !(num & (num - 1)));

}
    //parses integer from input str if greater than 0 or a power of 2
    //if error: returns -1
    uint32_t getValidInteger(std::string str)
    {
        uint32_t retVal = -1;

        if (!str.empty() && std::stoi(str) > 0)
        {
            retVal = std::stoi(str);
            //check if not power of two
            if (!isPowerOfTwo(retVal))
            {
                return -1;
            }
        }
        return retVal;
    }

    //prints out results with required formatting
    void displayResults(uint32_t loadHits, uint32_t loadMisses, uint32_t storeHits, uint32_t storeMisses)
    {
        uint32_t loads, stores, cycles = 0U;
        loads = loadMisses + loadHits;
        stores = storeMisses + storeHits;
        cycles = loads + stores;

        std::cout << "Total loads: " << loads << std::endl;
        std::cout << "Total stores: " << stores << std::endl;
        std::cout << "Load hits: " << loadHits << std::endl;
        std::cout << "Load misses: " << loadMisses << std::endl;
        std::cout << "Store hits: " << storeHits << std::endl;
        std::cout << "Store misses: " << storeMisses << std::endl;
        std::cout << "Total cycles: " << cycles << std::endl;
        return;
    }

    int printErrorMsg(const std::string &errorMsg) {
        std::cerr << errorMsg << std::endl;
        exit(1);
    }
    //input parsing functions


    void displayResults(int loadHits, int loadMisses, int storeHits, int storeMisses, int totalCycles);

    int createCacheIfArgsValid(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5,
                               std::string s6);
}