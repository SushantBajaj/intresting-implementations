#include <iostream>
#include "hyper_log_log.h"
#include <random>

uint64_t Random64(){
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}

void test0(){
    try{
        HyperLogLog hll(10);
        std::cout << "[PASS] Constructor test "<<std::endl;
    }catch(...){
        std::cout << "[FAIL] Constructor test"<<std::endl;
    }
}

void test1(){
    HyperLogLog hll(5);
    int result = hll.TestleadingZeros(2,0);
    if(result-1 == 62){
        std::cout << "[PASS] Leading Zero test"<<std::endl;
    }else{
        std::cout << "[FAIL] Leading Zero test"<<std::endl;
    }
}

void test2(){
    HyperLogLog hll(5);
    int result = hll.TestgetMsb(((size_t)5<<61),3);
    if(result == 5){
        std::cout << "[PASS] MSB test"<<std::endl;
    }else{
        std::cout << "[FAIL] MSB test"<<std::endl;
    }
}

void test3(){
    try{
        HyperLogLog hll(5);
        for(int i =0;i<10000;i++){
            hll.insert(Random64());
        }
        hll.estimateCardinality();
        std::cout << "[PASS] Basic test"<<std::endl;
    }catch(...){
        std::cout << "[FAIL] Basic test"<<std::endl;

    }
}






int main(){
    test0();
    test1();
    test2();
    test3();
}


// todo change assert to throw error