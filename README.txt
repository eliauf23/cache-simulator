We used the gcc.trace and swim.trace for testing the best cache as the 
data is from running a real program and is more representative of real program data.
The other more contrived files are useful for verifying whether the program is working as intended.

Cache size: compare the performance of large vs small caches
- benefits of large: store more data, drawbacks: more costly for materials & more power consumption and searching (this discussion is more complicated)

Larger total cache sizes had a lower miss rate?
Fewer (total) cycles because fewer evictions per total operations

Another major consideration - Time!!!
We used <chrono> to measure the number of ms it took for a program to run 
when our program was timing out on gradescope. 
Even though the fully associative caches had comparatively fewer misses & fewer total cycles,
it took significantly longer to run (up to even 4-6x the amount of time of an N-way-associative cache)

For each config=(S, N, B = 16) - discuss all 6 cache configurations. Hold num bytes constant - 16

(numSets, 1, 16) = direct-mapped cache
(numSets, numBlocksPerSet, 16) = N-way-associative
(1, numBlocksPerSet , 16)  = fully assoc


# LRU vs FIFO - not much of a difference?
# no-write-allocate vs write-allocate: 
# write-through vs write-back

To determine which cache configuration has the best 
overall effectiveness, we chose cache size C = numBlocks*numSets*blockSize = 2^16


hit rate = 
num cycles

experiment: choose cache with constant size = C bytes

run all cache configuration on trace (6 bc cant have no-write-alloc & write back)

- using <chrono>, we timed (in ms) how long it takes for the simulator to run on each configuration
while obviously, this isn't an exact metric reflecting the performance of this configuration as other tasks
play into this