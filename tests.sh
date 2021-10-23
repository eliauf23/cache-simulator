#!/bin/sh

make clean

make

#./csim 256 4 16 write-allocate write-through lru < example_traces/swim.trace

./csim 256 4 16 write-allocate write-back lru < example_traces/read02.trace
./csim 2 4 16 write-allocate write-back lru < example_traces/read03.trace
./csim 256 4 16 write-allocate write-through lru < example_traces/write01.trace
./csim 256 4 16 write-allocate write-back lru < example_traces/write02.trace
#./csim 256 4 16 write-allocate write-back lru < example_traces/gcc.trace


#./csim 128 8 32 no-write-allocate write-through lru < example_traces/read01.trace

#./csim 2 4 4 write-allocate write-back lru < example_traces/write01.trace

#should enter tests for other configurations - direct, N-way assoc, fully assoc

# echo "enter numSets numBlocks blockSize & filename:\n"


# echo "write-allocate write-through lru \n\n"
# ./csim $1 $2 $3 write-allocate write-through lru < example_traces/$4 

# echo "write-allocate write-back lru \n\n"
# ./csim $1 $2 $3 write-allocate write-back lru < example_traces/$4

# echo "no-write-allocate write-through lru \n\n"
# ./csim $1 $2 $3 no-write-allocate write-through lru < example_traces/$4

# echo "no-write-allocate write-back lru: this configuration should produce an error! \n\n"
# ./csim $1 $2 $3 no-write-allocate write-through lru < example_traces/$4
