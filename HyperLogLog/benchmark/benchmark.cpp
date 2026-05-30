#include "hyper_log_log.h"

#include <iostream>
#include <vector>
#include <unordered_set>
#include <chrono>
#include <random>
#include <cmath>


uint64_t Random64(){
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}

std::string Scientific(size_t x){

    if(x == 1000000) return "10^6";
    if(x == 10000000) return "10^7";
    if(x == 100000000) return "10^8";

    return std::to_string(x);
}

void Benchmark(size_t size,int register_bits){

    size_t registers = (size_t)1<<register_bits;

    std::cout<<"========================================"<<std::endl;
    std::cout<<"cardinality = "<<Scientific(size)<<std::endl;
    std::cout<<"register bits = "<<register_bits<<std::endl;
    std::cout<<"registers = "<<registers<<std::endl;

    std::cout<<"hll memory = "
             <<(registers*sizeof(int))/1024.0
             <<" KB"<<std::endl;

    std::cout<<"hashmap memory (minimum) = "
             <<(size*sizeof(uint64_t))
                /(1024.0*1024.0)
             <<" MB"<<std::endl;

    std::cout<<"========================================"<<std::endl;

    std::vector<uint64_t> values(size);

    for(size_t i =0;i<size;i++){
        values[i] = Random64();
    }

    HyperLogLog hll(register_bits);
    std::unordered_set<uint64_t> hashmap;

    auto start = std::chrono::high_resolution_clock::now();

    for(auto &i : values){
        hll.insertHash(i);
    }

    auto end = std::chrono::high_resolution_clock::now();

    auto hll_insert_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end-start
        ).count();

    start = std::chrono::high_resolution_clock::now();

    for(auto &i : values){
        hashmap.insert(i);
    }

    end = std::chrono::high_resolution_clock::now();

    auto hashmap_insert_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end-start
        ).count();

    start = std::chrono::high_resolution_clock::now();

    long long estimate =
        hll.estimateCardinality();

    end = std::chrono::high_resolution_clock::now();

    auto estimate_time =
        std::chrono::duration_cast<std::chrono::microseconds>(
            end-start
        ).count();

    double error =
        std::abs((double)estimate-size)
        *100.0
        /size;

    std::cout<<"actual cardinality = "
             <<size
             <<std::endl;

    std::cout<<"estimated cardinality = "
             <<estimate
             <<std::endl;

    std::cout<<"error = "
             <<error
             <<"%"
             <<std::endl;

    std::cout<<std::endl;

    std::cout<<"hashmap insertion time = "
             <<hashmap_insert_time/1000000.0
             <<" s"
             <<std::endl;

    std::cout<<"hll insertion time = "
             <<hll_insert_time/1000000.0
             <<" s"
             <<std::endl;

    std::cout<<"hll estimate time = "
             <<estimate_time/1000000.0
             <<" s"
             <<std::endl;

    std::cout<<std::endl;

    std::cout<<"hashmap/hll speedup = "
             <<(double)hashmap_insert_time
                /hll_insert_time
             <<"x"
             <<std::endl;

    std::cout<<"========================================"<<std::endl;
    std::cout<<std::endl;
}

int main(){

    std::cout<<"Scaling Benchmark"<<std::endl;
    std::cout<<std::endl;

    Benchmark(1000000,10);
    Benchmark(10000000,15);
    Benchmark(100000000,20);

    std::cout<<"Register Sensitivity Benchmark"<<std::endl;
    std::cout<<std::endl;

    Benchmark(1000000,4);
    Benchmark(1000000,8);
    Benchmark(1000000,12);
    Benchmark(1000000,16);
}