#include "csim_fns.h"
#include "cache.h"
#include <bitset>
#include <cstdint>
#include <cstring>



    unsigned int isPowerOfTwo(uint32_t num)
    {
        return (num && !(num & (num - 1)));
    }



    // parses integer from input str if greater than 0 or a power of 2
    // if error: returns -1
    int getValidInteger(std::string str)
    {
        int retVal = -1;

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



    //prints given error message to std:err
    int printErrorMsg(const std::string &errorMsg)
    {
        std::cerr << errorMsg << std::endl;
        return 1;;
    }



    // input parsing functions
  int checkIfArgsValid( char * s1,  char * s2,  char * s3,  char * s4,  char * s5,
                          char * s6)
    {

        uint32_t blockSize = 0U;


        // modularize all of the following into argument parsing/checking functions
        if (strcmp(s1, "") != 0 && atol(s1) > 0)
        {
            // check if not power of two
            if (!isPowerOfTwo(atol(s1)))
            {
                printErrorMsg("Num sets is not power of 2.");
                return 1;;
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            return 1;;
        }

        // determine if block number is valid
        if (strcmp(s2, "") != 0  && atol(s2) > 0)
        {

            if (!isPowerOfTwo(atol(s2)))
            {
                // check if not power of two
                printErrorMsg("Num blocks is not power of 2.");
                return 1;;
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            return 1;;
        }

        if (strcmp(s3, "") != 0  && atol(s3) > 0)
        {
            blockSize = atol(s3);
            if (!isPowerOfTwo(blockSize))
            {
                // check if not power of two
                printErrorMsg("cache size not power of 2.");
                return 1;
            }
            // error checking
            if (blockSize < MIN_BLOCK_SIZE)
            {
                printErrorMsg("Invalid block size");
                return 1;;
            }
        }
        else
        {
            printErrorMsg("Invalid cache features.");
            return 1;;
        }

        if ((strcmp(s4, "no-write-allocate") != 0) && (strcmp(s4, "write-allocate") != 0))
        {
            printErrorMsg("Problem with arg 4");
            return 1;;
        }

        if ((strcmp(s5, "write-through") != 0) && (strcmp(s5, "write-back") != 0))
        {
            printErrorMsg("Problem with arg 5");
            return 1;;
        }

        if ((strcmp(s6, "fifo") != 0) && (strcmp(s6, "lru") != 0))
        {
            printErrorMsg("Problem with arg 6");
            return 1;;
        }

        if ((strcmp(s4, "no-write-allocate") == 0)  && (strcmp(s5, "write-back") == 0))
        {
            printErrorMsg("Contradictory arguments");
            return 1;;
        }

        return 0;
    }