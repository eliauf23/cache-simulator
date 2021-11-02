/*
 * Header file for cache simulator helper functions.
 * CSF Assignment 3
 * Elizabeth Aufzien eaufzie1@jh.edu
 * Rosie Wolkind rwolkin1@jh.edu
 */

#ifndef CSIM_FNS_H
#define CSIM_FNS_H

#define MIN_BLOCK_SIZE 4U

#include <iostream>
#include <string>
#include <cstring>

/* Parses integer from input str if greater than 0 and a power of 2.
 *
 * Parameters: std::string str - Input string
 *
 * Returns: if error return -1, else returns valid positive integer that's a power of 2.
 */
int getValidInteger(std::string str);

/*
 * Helper function to check if number is a power of 2 with bitwise/logical operations.
 *
 * Parameters: uint32_t num - number you want to check.
 *
 * Returns: 0 if the number is a power of 2, else returns 1.
 *
 */

unsigned int isPowerOfTwo(uint32_t num);

/*
 * Prints error message to std error & returns 1
 *
 * Parameters: message
 *
 * Returns: 1 (always)
 *
 */
int printErrorMsg(const std::string &errorMsg);

/*
 * Goes through the six args and returns 0 if they are individually valid && no illegal combinations
 *
 * Parameters: 6 strings
 *
 * Returns: returns 1 & prints an error message if any of the args are invalid or if there are illegal combinations.
 *
 */
int checkIfArgsValid(char *s1, char *s2, char *s3, char *s4, char *s5,
                     char *s6);

#endif // CSIM_FNS_H