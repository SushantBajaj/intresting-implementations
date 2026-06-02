#pragma once

#include <cstdint>
#include <cstddef>
#include <vector>

class CountMinSketch{
    public:
    CountMinSketch(size_t width, size_t hash_functions);
    void incrementHash(uint64_t hash);
    uint64_t estimateUpperBoundOfHash(uint64_t hash) const;
    void clear();
    void merge(const CountMinSketch &other);

    private:
    size_t width_;
    size_t hash_functions_;
    std::vector<uint64_t> counters_;
};