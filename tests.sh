#!/bin/sh

make clean

make



./csim 256 16 64 no-write-allocate write-through lru < example_traces/swim.trace



./csim 256 16 64 write-allocate write-through lru < example_traces/swim.trace




./csim 256 16 64 write-allocate write-back lru < example_traces/swim.trace



./csim 256 16 64 no-write-allocate write-through fifo < example_traces/swim.trace

./csim 256 16 64 write-allocate write-through fifo < example_traces/swim.trace



./csim 256 16 64 write-allocate write-back fifo < example_traces/swim.trace




#basic tests
#./csim 2 2 4 write-allocate write-through lru < example_traces/ex1.trace

#gradescope tests

echo "\n---test 1---------\n"
./csim 1 1 4 write-allocate write-through lru < example_traces/swim.trace
#echo "\n---test 1---------\n"
#./csim 1 1 4 write-allocate write-through lru < example_traces/read02.trace


#echo "\n---test 2---------\n"
#./csim 2 8192 16 no-write-allocate write-through lru < example_traces/read01.trace

echo "\n---test 2---------\n"
./csim 256 16 64 no-write-allocate write-through lru < example_traces/swim.trace
# ./csim 256 16 64 no-write-allocate write-through lru < example_traces/write02.trace


#echo "\n---test 3---------\n"

#./csim 128 32 64 write-allocate write-through lru < example_traces/swim.trace


# ./csim 8192 1 16 no-write-allocate write-through lru < example_traces/swim.trace
# ./csim 8192 1 16 write-allocate write-through lru < example_traces/swim.trace
# ./csim 8192 1 16 write-allocate write-back lru < example_traces/swim.trace
# ./csim 2048 4 16 no-write-allocate write-through lru  < example_traces/swim.trace
# ./csim 2048 4 16 write-allocate write-through lru < example_traces/swim.trace
# ./csim 2048 4 16 write-allocate write-back lru < example_traces/swim.trace
# ./csim 1 8192 16 no-write-allocate write-through lru < example_traces/swim.trace
# ./csim 1 8192 16 write-allocate write-through lru < example_traces/swim.trace
# ./csim 1 8192 16 write-allocate write-back lru < example_traces/swim.trace

# ./csim 1024 1 128 no-write-allocate write-through lru < example_traces/swim.trace
# ./csim 1024 1 128 write-allocate write-through lru < example_traces/swim.trace
# ./csim 1024 1 128 write-allocate write-back lru < example_traces/swim.trace

# ./csim 256 4 128 no-write-allocate write-through lru < example_traces/swim.trace
# ./csim 256 4 128 write-allocate write-through lru < example_traces/swim.trace
# ./csim 256 4 128 write-allocate write-back lru < example_traces/swim.trace

# ./csim 1 1024 128 no-write-allocate < example_traces/swim.trace
# ./csim 1 1024 128 write-allocate write-through lru < example_traces/swim.trace
# ./csim 1 1024 128 write-allocate write-back lru < example_traces/swim.trace

#./csim 256 4 128 write-allocate write-back lru < example_traces/swim.trace
#./csim 256 16 64 no-write-allocate write-through lru < example_traces/swim.trace
#./csim 256 4 16 write-allocate write-through lru < example_traces/swim.trace
#./csim 256 4 16 write-allocate write-back lru < example_traces/read02.trace
#./csim 2 4 16 write-allocate write-back lru < example_traces/read03.trace
#./csim 256 4 16 write-allocate write-through lru < example_traces/write01.trace
#./csim 256 4 16 write-allocate write-back lru < example_traces/write02.trace
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
