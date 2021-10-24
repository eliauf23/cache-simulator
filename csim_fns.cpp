#include "csim_fns.h"
#include "cache.h"
#include <string>
#include <bitset>
#include <cstdint>


namespace CacheSimulator
{

    unsigned int isPowerOfTwo(uint32_t num)
    {
        return (num && !(num & (num - 1)));
    }
    // parses integer from input str if greater than 0 or a power of 2
    // if error: returns -1
    uint32_t getValidInteger(std::string str)
    {
        uint32_t retVal = -1;

        if (!str.empty() && std::stoi(str) > 0)
        {
            retVal = std::stoi(str);
            // check if not power of two
            if (!isPowerOfTwo(retVal))
            {
                return -1;
            }
        }
        return retVal;
    }

    int printErrorMsg(const std::string &errorMsg)
    {
        std::cerr << errorMsg << std::endl;
        exit(1);
    }
    // input parsing functions

    int checkIfArgsValid(std::string s1, std::string s2, std::string s3, std::string s4, std::string s5,
                         std::string s6)
    {

        uint32_t blockSize;

        // modularize all of the following into argument parsing/checking functions
        if (!s1.empty() && std::stoi(s1) > 0)
        {
            // check if not power of two
            if (!isPowerOfTwo(std::stoi(s1)))
            {
                CacheSimulator::printErrorMsg("Num sets is not power of 2.");
                exit(1);
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            exit(1);
        }

        // determine if block number is valid
        if (!s2.empty() && std::stoi(s2) > 0)
        {

            if (!isPowerOfTwo(std::stoi(s2)))
            {
                // check if not power of two
                printErrorMsg("Num blocks is not power of 2.");
                exit(1);
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            exit(1);
        }

        if (!s3.empty() && std::stoi(s3) > 0)
        {
            blockSize = std::stoi(s3);
            if (!isPowerOfTwo(blockSize))
            {
                // check if not power of two
                printErrorMsg("cache size not power of 2.");
                exit(1);
            }
            // error checking
            if (blockSize < MIN_BLOCK_SIZE)
            {
                printErrorMsg("Invalid block size");
                exit(1);
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            exit(1);
        }

        if (s4 != "no-write-allocate" && "write-allocate" != s4)
        {
            printErrorMsg("Problem with arg 4");
            exit(1);
        }

        if (s5 != "write-through" && "write-back" != s5)
        {
            printErrorMsg("Problem with arg 5");
            exit(1);
        }

        if (s6 != "fifo" && s6 != "lru")
        {
            printErrorMsg("Problem with arg 6");
            exit(1);
        }

        if (s4 == "no-write-allocate" && "write-back" == s5)
        {
            printErrorMsg("Contradictory arguments");
            exit(1);
        }

        return 0;
    }
}