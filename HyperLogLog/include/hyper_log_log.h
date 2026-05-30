#pragma once
#include <vector>
#include <cstddef>

class HyperLogLog{
    private:
    int register_bits;
    size_t registers;
    int bits_in_hash;
    double alpha;
    std::vector<int> sketch;

    void calculateAlpha();
    int leadingZeros(size_t hash_value, int n);
    size_t getMsb(size_t hash_value, int n);
    
    public:
    
    HyperLogLog(int regsiter_bits);
    
    void insertHash(size_t hash_value);
    long long estimateCardinality();


    int TestleadingZeros(size_t hash_value, int n);
    size_t TestgetMsb(size_t hash_value, int n);

};