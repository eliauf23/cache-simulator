#include "csim_fns.h"
#include "cache.h"
#include "block.h"
#include <string>
#include <cstdlib>
#include <iostream>
#include <cstdio>
#include <cstring>
#include <chrono>


#define NUM_ARGS 7

using std::cin;
using std::cout;
using std::endl;
using std::getline;
using std::stoul;
using std::string;


int main(int argc, char *argv[])
{

    if (argc != NUM_ARGS)
    {
        return printErrorMsg("incorrect num args");
    }
    // ensures no illegal combinations of args & all valid before parsing
    else if (checkIfArgsValid(argv[1], argv[2], argv[3], argv[4], argv[5], argv[6]) != 0)
    {
        return printErrorMsg("Invalid args. Try again.");
    }
    else
    {

        // parse all command line args
        uint32_t numSets = getValidInteger(argv[1]);
        uint32_t blocksPerSet = getValidInteger(argv[2]);
        uint32_t blockSize = getValidInteger(argv[3]);
        // enums from cache.cpp
        CacheSimulator::Allocation alloc;
        CacheSimulator::Write write;
        CacheSimulator::Eviction evict;

        if (strcmp(argv[4], "no-write-allocate") == 0)
            alloc = CacheSimulator::NO_WRITE_ALLOCATE;
        else
            alloc = CacheSimulator::WRITE_ALLOCATE;

        if (strcmp(argv[5], "write-through") == 0)
            write = CacheSimulator::WRITE_THROUGH;
        else
            write = CacheSimulator::WRITE_BACK;

        if (strcmp(argv[6], "fifo") == 0)
            evict = CacheSimulator::FIFO;
        else
            evict = CacheSimulator::LRU;

        // initialize cache
        CacheSimulator::Cache cache = CacheSimulator::Cache(numSets, blocksPerSet, blockSize, alloc, write, evict);

        char operation = 'd'; // default
        uint32_t address;     // hex address
        string line;
        int num;

        // read in file


        while (cin)
        {
            cin >> operation >> std::hex >> address >> num;
            if (operation == 'd') break; // i.e. if file is empty/you hit end of file without errors

            uint32_t index = cache.getIndexFromAddress(address);
            uint32_t tag = cache.getTagFromAddress(address);
            // STORE!
            if (operation == 's')
            {

                cache.store(index, tag);

            }

            // LOAD!

            else if (operation == 'l')
            {

                cache.load(index, tag);



            }

            // ERROR!

            else
            {
                return printErrorMsg("issue with trace file");
            }
           
           
            operation = 'd';
        }



        cache.printResults();
        return 0;
    }
}