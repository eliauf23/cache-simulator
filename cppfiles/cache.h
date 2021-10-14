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

    Cache::handleWrite() {

        if()

    }



}
#endif  //!__CACHE__H__