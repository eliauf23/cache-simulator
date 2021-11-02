Elizabeth Aufzien, eaufzie1@jh.edu
Rosie Wolkind, rwolkin1@jh.edu

# CSF Assignment 3: Cache Simulator


## Discussion

### C=SNB, S = 256, N = 16, B = 64 

|                      | no-write-allocate write-through lru | write-allocate write-through lru | write-allocate write-back lru | no-write-allocate write-through fifo | write-allocate write-through fifo | write-allocate write-back fifo |   |   |   |
|-------------------------|-------------------------------------|----------------------------------|-------------------------------|--------------------------------------|-----------------------------------|--------------------------------|---|---|---|
| Total Loads             | 220668                              | 220668                           | 220668                        | 220668                               | 220668                            | 220668                         |   |   |   |
| Total stores            | 82525                               | 82525                            | 82525                         | 82525                                | 82525                             | 82525                          |   |   |   |
| Load Hits               | 218970                              | 220275                           | 220275                        | 218970                               | 220275                            | 220275                         |   |   |   |
| Load Misses             | 1698                                | 393                              | 393                           | 1698                                 | 393                               | 393                            |   |   |   |
| Store Hits              | 67754                               | 79465                            | 79465                         | 67754                                | 79464                             | 79464                          |   |   |   |
| Store Misses            | 14771                               | 3060                             | 3060                          | 14771                                | 3061                              | 3061                           |   |   |   |
| Total Cycles            | 11257722                            | 14082236                         | 5851996                       | 11257722                             | 14083835                          | 5850396                        |   |   |   |
| Miss rate (1- hit rate) | 1 - 0.945681 = 0.054319             | 1- 0.988611 = 0.011389           | 1- 0.988611 = 0.011389        | 1 - 0.945681 = 0.054319              | 1-0.988608 = 0.011392             | 1-0.988608 = 0.011392          |   |   |   |
| Miss penalty            | 1600                                | 1600                             | 1600                          | 1600                                 | 1600                              | 1600                           |   |   |   |


### min cycles: write-allocate write-back fifo
### lowest miss rate: write-allocate write-through lru &	write-allocate write-back lru are tied
#### miss penalty is the same for all of them as their block size is the same

# LRU vs FIFO

LRU requires more upkeep, however, LRU overall had a lower miss rate & misses are costly. Updating some more counters is trivial compared to evicting blocks only to bring them into the cache again. This makes sense because FIFO only accounts for the time the block is loaded into the cache.

# Write Through vs Write Back

Doesn't impact hit/miss rates. Only impacts number of cycles/time.

Write back will be faster as you write to memory when the block is evicted (and it's dirty) so you can avoid a lot of work at the expense of having to keep track of a dirty bit. Write through has a high upfront cost, and it's redeeming factor is its simplicity and the fact that you can always have an updated copy of information in main memory AND the cache.  

# Write-Allocate vs No-Write-Allocate

Write allocate is conceptually better because it can be paired with write-back (which has faster performance) and because often programs access the same block multiple times sequentially or almost sequentially. By bringing the block into the cache, you make future accesses/writes easier. Whereas no-write-allocate is good when you will not be accessing data in a block soon. However, you can't use no-write-allocate with write-back so you'll have to write through and that takes a long time.

With this, to get the best cache configuration:

- choose constant block size
- let cache be write-back, write-alloc & LRU
- Keep cache size constant & vary number of sets & number of blocks
- Try to make cache increasingly associative without having major wait times/stalling
## Contributions: 
- Tasks:
    - validating command line arguments
    - writing helper functions such as log_base2, isPowerofTwo, isValidInteger, printErrorMsg
    - making Block, Cache (& Set - which we removed after MS2) classes
    - parsing data from trace files
    - converting address to index & tag
    - Designing handler functions for load/store hit/miss
    - Making sure that we updated loadHits, loadMisses, storeHits, storeMisses
    - Determining control flow for updating cycles
    - printing out properly formatted results
    - LRU counter updating
    - FIFO counter updating
- Debugging!!!
- Commenting code
- Fixing memory leaks
