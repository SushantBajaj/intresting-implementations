#pragma once

#include <vector>
#include <cstddef>
#include <cstdint>

class BloomFilter{

    public:
    BloomFilter(size_t bits, size_t hash_functions);
    void insertHash(uint64_t hash);
    bool containsHash(uint64_t hash);

    private:
    size_t bits_;
    size_t hash_functions_;
    std::vector<bool>bit_array_;

}