#pragma once

#include <atomic>
#include <cstdint>
#include <cstddef>
#include <mutex>
#include <shared_mutex>

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
    std::atomic_uint64_t* counters_;
    mutable std::shared_mutex mtx;

    std::unique_lock<std::shared_mutex> getDeferLock() const;
    std::unique_lock<std::shared_mutex> acquireLock() const;
    void copyAtomicCounter(std::atomic_uint64_t* destination, std::atomic_uint64_t* source, size_t element_count);
};