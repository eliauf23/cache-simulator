October/November Madness:

To determine which cache configuration has the best 
overall effectiveness, we chose cache size C = numBlocks*numSets*blockSize = 2^16


hit rate = 
num cycles

experiment: choose cache with constant size = C bytes

run all cache configuration on trace (6 bc cant have no-write-alloc & write back)

- using chrono, we timed (in ms) how long it takes for the simulator to run on each configuration
while obviously, this isn't an exact metric reflecting the performance of this configuration as other tasks
play into this