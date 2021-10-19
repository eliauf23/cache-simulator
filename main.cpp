#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include "set.h"
#include <string>
#include <bitset>
#include <sstream>
#include <limits>
#include <cstdio>
#include <cstdint>
#include <iostream>

#define NUM_ARGS 7

int main(int argc, char **argv)
{

    std::string s1 = std::string(argv[1]);
    std::string s2 = std::string(argv[2]);
    std::string s3 = std::string(argv[3]);
    std::string s4 = std::string(argv[4]);
    std::string s5 = std::string(argv[5]);
    std::string s6 = std::string(argv[6]);

    bool argsValid = CacheSimulator::checkIfArgsValid(s1, s2, s3, s4, s5, s6) == 0;

    if (argc == NUM_ARGS && argsValid)
    {

        uint32_t numSets = CacheSimulator::getValidInteger(s1);
        uint32_t numBlocks = CacheSimulator::getValidInteger(s2);
        uint32_t blockSize = CacheSimulator::getValidInteger(s3);
        CacheSimulator::Allocation alloc;
        CacheSimulator::Write write;
        CacheSimulator::Eviction evict;

        if (s4 == "no-write-allocate")
            alloc = CacheSimulator::NO_WRITE_ALLOCATE;
        else
            alloc = CacheSimulator::WRITE_ALLOCATE;

        if (s5 == "write-through")
            write = CacheSimulator::WRITE_THROUGH;
        else
            write = CacheSimulator::WRITE_BACK;

        if (s6 == "fifo")
            evict = CacheSimulator::FIFO;
        else
            evict = CacheSimulator::LRU;

        CacheSimulator::Cache cache = CacheSimulator::Cache(numSets, numBlocks, blockSize, alloc, write, evict);

        std::string operation; //load or store
        uint32_t address;      //hex address
        uint32_t unusedNum;    //thing @ end of the line
        std::string line;

        while (std::getline(std::cin, line))
        {
            std::istringstream string_stream(line);
            string_stream >> operation >> std::hex >> address >> std::dec >> unusedNum;

            if (operation == "s")
            {
                std::cout << "store" << std::endl;

                //handleStoreMiss() i.e. set is empty
                //num blocks = N in N-way associativity
                if (cache.find(address) == cache.getNumBlocks())
                {
                    std::cout << "handle store miss" << std::endl;

                    cache.handleStoreMiss(address);
                    //handleStoreHit
                }
                else
                { // cache hit, findAddress contains block number
                    std::cout << "handle store hit" << std::endl;

                    cache.handleStoreHit(address);
                }
            }
            else if (operation == "l")
            {
                if (cache.find(address) == cache.getNumBlocks())
                {
                    cache.handleLoadMiss(address);
                    std::cout << "handle load miss" << std::endl;
                }
                else
                {
                    //handle load hit
                    cache.handleLoadHit(address);
                    std::cout << "handle load hit" << std::endl;
                }
            }
            else
            {
                //error!
                return CacheSimulator::printErrorMsg("Invalid operation");
            }
        }
        cache.printResults();

        return 0;
    }
    else
    {
        return CacheSimulator::printErrorMsg("Invalid cmd line args.");
    }
}