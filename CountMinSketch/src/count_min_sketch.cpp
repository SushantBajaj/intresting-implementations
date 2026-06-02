#include "count_min_sketch.h"
#include "hash.h"

#include <cstdint>
#include <cstddef>
#include <limits>
#include <stdexcept>
#include <algorithm>
#include <cstring>

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
        counters_ = new uint64_t[counters_size_]{};

}

CountMinSketch::CountMinSketch(const CountMinSketch& other):
    width_(other.width_),
    hash_functions_(other.hash_functions_),
    counters_size_(other.counters_size_){

    this->counters_ = new uint64_t[this->counters_size_];
    memcpy(this->counters_, other.counters_, (this->counters_size_*(sizeof(uint64_t))));

}

CountMinSketch& CountMinSketch::operator=(const CountMinSketch& other){
    if(&other==this) return *this;
    delete [] this->counters_;
    this->width_ = other.width_;
    this->hash_functions_ = other.hash_functions_;
    this->counters_size_ = other.counters_size_;
    this->counters_ = new uint64_t[this->counters_size_]{};
    memcpy(this->counters_,other.counters_, (counters_size_*sizeof(uint64_t)));
    return *this;
}

CountMinSketch::CountMinSketch(CountMinSketch&& other) noexcept :
    width_(other.width_),
    hash_functions_(other.hash_functions_),
    counters_size_(other.counters_size_),
    counters_(other.counters_) {
    
    other.width_ = {};
    other.hash_functions_ = {};
    other.counters_size_ = {};
    other.counters_ = {};
}

CountMinSketch& CountMinSketch::operator=(CountMinSketch&& other) noexcept{
    if(this==&other) return *this;

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




void CountMinSketch::incrementHash(const uint64_t hash){
    uint64_t hash2 = SplitMix64(hash);
    for(size_t i =0; i<hash_functions_;i++){
        size_t row_idx = (((hash + i)* hash2) % width_);
        size_t counters_idx = (i*width_) + row_idx;
        counters_[counters_idx]++;
    }
}

uint64_t CountMinSketch::estimateUpperBoundOfHash(const uint64_t hash) const{
    uint64_t hash2 = SplitMix64(hash);
    uint64_t estimation = std::numeric_limits<uint64_t>::max();
    for(size_t i =0; i<hash_functions_;i++){
        size_t row_idx = (((hash + i)* hash2) % width_);
        size_t counters_idx = (i*width_) + row_idx;
        estimation = std::min(estimation, counters_[counters_idx]);
    }
    return estimation;
}

void CountMinSketch::clear(){
    std::fill(counters_,counters_+counters_size_,0);
}


void CountMinSketch::merge(const CountMinSketch &other){
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