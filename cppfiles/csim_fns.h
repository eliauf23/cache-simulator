#ifndef __CSIM_FNS__H__
#define __CSIM_FNS__H__

class CacheSimulator {
    private:
        uint32_t _numSets;
        uint32_t _numBlocks;
        uint32_t _blockSize;
        uint32_t _storePolicy;
        uint32_t _writePolicy;
        uint32_t _evictionPolicy;

    CacheSimulator() {};
    CacheSimulator(numSets, numBlocks, blockSize, storePolicy, writePolicy, evictionPolicy) {
        _numSets = numSets;
        _numBlocks = numBlocks;
        _blockSize = blockSize;
        _storePolicy = storePolicy;
        _writePolicy = writePolicy;
        _evictionPolicy = evictionPolicy;
    };

    public:
}


#endif  //!__CSIM_FNS__H__