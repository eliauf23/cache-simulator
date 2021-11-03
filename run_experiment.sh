#!/bin/sh



make clean
make 
echo "Let the hunger games begin: \n"



./csim $1 $2 $3 write-allocate write-back fifo < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_writealloc_writeback_fifo.txt


./csim $1 $2 $3 write-allocate write-through fifo < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_writealloc_writethru_fifo.txt


./csim $1 $2 $3 no-write-allocate write-through fifo < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_nowritealloc_writethru_fifo.txt


./csim $1 $2 $3 write-allocate write-back lru < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_writealloc_writeback_lru.txt


./csim $1 $2 $3 write-allocate write-through lru < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_writealloc_writethru_lru.txt


./csim $1 $2 $3 no-write-allocate write-through lru < example_traces/gcc.trace >> test_results/$1_$2_$3/gcc_$1_$2_$3_nowritealloc_writethru_lru.txt



./csim $1 $2 $3 write-allocate write-back fifo < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_writealloc_writeback_fifo.txt


./csim $1 $2 $3 write-allocate write-through fifo < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_writealloc_writethru_fifo.txt


./csim $1 $2 $3 no-write-allocate write-through fifo < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_nowritealloc_writethru_fifo.txt


./csim $1 $2 $3 write-allocate write-back lru < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_writealloc_writeback_lru.txt


./csim $1 $2 $3 write-allocate write-through lru < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_writealloc_writethru_lru.txt


./csim $1 $2 $3 no-write-allocate write-through lru < example_traces/swim.trace >> test_results/$1_$2_$3/swim_$1_$2_$3_nowritealloc_writethru_lru.txt
