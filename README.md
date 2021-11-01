## C=SNB, S = 256, N = 16, B = 64 

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


# min cycles: write-allocate write-back fifo
# lowest miss rate: write-allocate write-through lru &	write-allocate write-back lru are tied
# miss penalty is the same for all of them as their block size is the same