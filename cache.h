#ifndef __CACHE__H__
#define __CACHE__H__



class Cache {

    public:
    Cache() {};
    Cache(uint32_t num_sets, uint32_t num_blocks, uint32_t block_size, uint32_t miss_strategy, uint32_t write_strategy, uint32_t evict_strategy) {
        this.num_sets = num_sets;
        this.num_blocks = num_blocks;
        this.block_size = block_size;
        this.miss_strategy = miss_strategy;
        this.write_strategy = write_strategy;
        this.evict_strategy = evict_strategy;
    }

    //returns int
    Cache::findAddressInCache(cache, address){
        // preform hash function to see if correlating block has address
        //If yes, return 1
        //If no, return 0
        //Don't update any counters //TODO: check if we should update cycles...But i really don't think so
    }

    //return void
    Cache::handelStoreHit(cache, address, writeAllocate, writeBack) {
        //Take in all relevant parameters. 
        //Follow logic to update chache/not update
        //Only update cycles. No other parameter
            if(writeAllocate) {

                //TODO: handle write-allocate case
                //bring the relevant memory block into the cache before the store proceeds             
                //cycles += 100 (bc we access memory)


        } else { //is no-write allocate
                //TODO: handle no-write-allocate case by not modifying cache? (Yep, just exit here)

            }
    }

    //
    Cache::handleStoreMiss(cache, address, writeAllocate, writeBack){
                if(writeBack) {
                    //TODO: handle write-back case
                    //if store & cache hit & write back:
                    //you need to write to cache
                    //mark block as modified: dirty = 1
                    //TODO: make functions to compute the following:
                        // set_index = findIndex(cache, address);
                        //  block_index = findAddressInBlock(cache, address);
                    //if this block is evicted in the future - must be written back to memory before replacing

                } else { //is write through
                    //TODO: handle write-through case
                    //write to cache & write to memory!
                }
    

    Cache::handleLoadMiss(cache, address, writeAllocate, writeBack, LIFO, LRU);{
        //find corresponding cache block
        //is there data there?
            //Yes: Is WB == true?
                //if yes. Cycles += 100;
        //regardless, load address to cache. 
        //Mark block as valid. 
        //if writeback, mark as dirty
        //if writethrough, load to memory (aka Cycles += 100)

    }

}
#endif  //!__CACHE__H__