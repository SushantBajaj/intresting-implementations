#include "count_min_sketch.h"
#include "hash.h"

#include <cstdint>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cstring>
#include <atomic>
#include <mutex>
#include <shared_mutex>



CountMinSketch::CountMinSketch(size_t width, size_t hash_functions) :
    width_(width),
    hash_functions_(hash_functions){

        if(width==0){
            throw std::invalid_argument("width must be positive");
        }
        if(hash_functions==0){
            throw std::invalid_argument("number of hash functions must be positive");
        }
        if(hash_functions> std::numeric_limits<size_t>::max()/width){
            throw std::overflow_error("width * count of hash functions must fit in size_t");
        }

        counters_size_ = hash_functions*width;
        counters_ = new std::atomic_uint64_t[counters_size_]{};

}

CountMinSketch::CountMinSketch(const CountMinSketch& other){

    std::unique_lock<std::shared_mutex> other_gaurd = other.acquireLock();

    this->width_ = other.width_;
    this->hash_functions_ = other.hash_functions_;
    this->counters_size_ = other.counters_size_;
    this->counters_ = new std::atomic_uint64_t[this->counters_size_]{};
    copyAtomicCounter(this->counters_, other.counters_, this->counters_size_);
    //cant use memcpy coz its atomic_uint64_t and not just uint64_t

}

CountMinSketch& CountMinSketch::operator=(const CountMinSketch& other){
    if(&other==this) return *this;
    std::unique_lock<std::shared_mutex> other_gaurd = other.getDeferLock();
    std::scoped_lock combined_lock(mtx,other_gaurd);
    delete [] this->counters_;
    this->width_ = other.width_;
    this->hash_functions_ = other.hash_functions_;
    this->counters_size_ = other.counters_size_;
    this->counters_ = new std::atomic_uint64_t[this->counters_size_]{};
    copyAtomicCounter(this->counters_,other.counters_, counters_size_);
    return *this;
}

CountMinSketch::CountMinSketch(CountMinSketch&& other) noexcept{

    std::unique_lock<std::shared_mutex> other_gaurd = other.acquireLock();

    this->width_ = other.width_;
    this->hash_functions_ = other.hash_functions_;
    this->counters_size_ = other.counters_size_;
    this->counters_ = other.counters_;
    
    other.width_ = {};
    other.hash_functions_ = {};
    other.counters_size_ = {};
    other.counters_ = {};
}

CountMinSketch& CountMinSketch::operator=(CountMinSketch&& other) noexcept{
    if(this==&other) return *this;
    std::unique_lock<std::shared_mutex> other_gaurd = other.getDeferLock();
    std::scoped_lock combined_lock(mtx,other_gaurd);
    delete [] counters_;

    this->width_ = other.width_;
    this->hash_functions_ = other.hash_functions_;
    this->counters_size_ = other.counters_size_;
    this->counters_ = other.counters_;

    other.width_ = {};
    other.hash_functions_ = {};
    other.counters_size_ = {};
    other.counters_ = {};

    return *this;
}


CountMinSketch::~CountMinSketch(){
    delete [] counters_;
}


std::unique_lock<std::shared_mutex> CountMinSketch::getDeferLock() const{
    std::unique_lock<std::shared_mutex> lock(mtx,std::defer_lock);
    return lock;
}
std::unique_lock<std::shared_mutex> CountMinSketch::acquireLock() const{
    std::unique_lock<std::shared_mutex> lock(mtx);
    return lock;
}

void CountMinSketch::copyAtomicCounter(std::atomic_uint64_t* destination, std::atomic_uint64_t* source, size_t element_count){
    for(size_t i =0;i<element_count;i++){
        destination[i].store(source[i].load());
    }
}


void CountMinSketch::incrementHash(const uint64_t hash){
    uint64_t hash2 = SplitMix64(hash);
    std::shared_lock<std::shared_mutex> guard (mtx);
    /*shared lock so that multiple threads can access this method at once,
    since we use atomic to handle increments anyways, its thread safe*/
    for(size_t i =0; i<hash_functions_;i++){
        size_t row_idx = (((hash + i)* hash2) % width_);
        size_t counters_idx = (i*width_) + row_idx;
        counters_[counters_idx].fetch_add(1);
    }
}

uint64_t CountMinSketch::estimateUpperBoundOfHash(const uint64_t hash) const{
    uint64_t hash2 = SplitMix64(hash);
    uint64_t estimation = std::numeric_limits<uint64_t>::max();

    std::shared_lock<std::shared_mutex> guard (mtx);
    for(size_t i =0; i<hash_functions_;i++){
        size_t row_idx = (((hash + i)* hash2) % width_);
        size_t counters_idx = (i*width_) + row_idx;
        estimation = std::min(estimation, (uint64_t)counters_[counters_idx].load());
    }
    return estimation;
}

void CountMinSketch::clear(){
    std::lock_guard<std::shared_mutex> guard (mtx);
    std::fill(counters_,counters_+counters_size_,0);
}


void CountMinSketch::merge(const CountMinSketch &other){
    if(this==&other) return;
    
    std::unique_lock<std::shared_mutex> other_gaurd = other.getDeferLock(); 
    std::scoped_lock combined_lock(mtx,other_gaurd);

    if(other.width_!=this->width_){
        throw std::invalid_argument("both cms should have same width");
    }
    if(other.hash_functions_!=this->hash_functions_){
        throw std::invalid_argument("both cms should have same count of hash functions");
    }

    for(size_t i = 0;i<(width_*hash_functions_);i++){
        this->counters_[i] = this->counters_[i] + other.counters_[i];
    }
}


