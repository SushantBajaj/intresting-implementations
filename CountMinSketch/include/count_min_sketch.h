#pragma once

#include <cstdint>
#include <cstddef>

class CountMinSketch{
    public:
    CountMinSketch(size_t width, size_t hash_functions);

    CountMinSketch(const CountMinSketch& other);
    CountMinSketch& operator=(const CountMinSketch& other);

    CountMinSketch(CountMinSketch&& other) noexcept;
    CountMinSketch& operator=(CountMinSketch&& other) noexcept;

    ~CountMinSketch();

    void incrementHash(uint64_t hash);
    uint64_t estimateUpperBoundOfHash(uint64_t hash) const;
    void clear();
    void merge(const CountMinSketch &other);

    private:
    size_t width_;
    size_t hash_functions_;
    size_t counters_size_;
    uint64_t* counters_;
};