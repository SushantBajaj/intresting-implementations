#include <bloom_filter.h>
#include <hash.h>

#include <vector>
#include <cstddef>
#include <cstdint>

BloomFilter::BloomFilter(size_t bit_count, size_t hash_functions) :
    bits_(bit_count),
    hash_functions_(hash_functions),
    bit_array_(bit_count) {}

void BloomFilter::insertHash(uint64_t hash){
    uint64_t hash2 = SplitMix64(hash);
    for(size_t i =0;i<hash_functions_;i++){
        size_t index = (hash + (hash2*i))%bits_;
        bit_array_[index] = true;
    }
}

bool BloomFilter::containsHash(uint64_t hash){
    uint64_t hash2 = SplitMix64(hash);
    for(size_t i =0;i<hash_functions_;i++){
        size_t index = (hash + (hash2*i))%bits_;
        if(!bit_array_[index]){
            return false;
        }
    }
    return true;
}