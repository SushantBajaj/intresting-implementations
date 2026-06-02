#include "count_min_sketch.h"
#include <limits>
#include <iostream>

void test0(){
    try{
        CountMinSketch cms(10,4);
        std::cout<<"[PASS] Base Constructor test "<<std::endl;
    }catch(...){
        std::cout<<"[FAIL] Base Constructor test "<<std::endl;
    }
    try{
        CountMinSketch cms(0,4);
        std::cout<<"[FAIL] Zero width Constructor test "<<std::endl;
    }catch(...){
        std::cout<<"[PASS] Zero width Constructor test "<<std::endl;
    }
    try{
        CountMinSketch cms(4,0);
        std::cout<<"[FAIL] Zero hash functions Constructor test "<<std::endl;
    }catch(...){
        std::cout<<"[PASS] Zero hash functions Constructor test "<<std::endl;
    }
    try{
        CountMinSketch cms(std::numeric_limits<size_t>::max(),2);
        std::cout<<"[FAIL] Overflow Constructor test "<<std::endl;
    }catch(...){
        std::cout<<"[PASS] Overflow Constructor test "<<std::endl;
    }
}
void test1(){
    CountMinSketch cms(20,2);
    for(int i =0;i<10;i++){
        cms.incrementHash(2);
    }
    if(cms.estimateUpperBoundOfHash(2)==10){
        std::cout<<"[PASS] Basic test "<<std::endl;
    }else{
        std::cout<<"[FAIL] Basic test "<<std::endl;
    }
}

void test2(){
    CountMinSketch cms(20,2);
    for(int i =0;i<10;i++){
        cms.incrementHash(2);
        cms.incrementHash(22);
    }
    if(cms.estimateUpperBoundOfHash(2)>=10 && cms.estimateUpperBoundOfHash(22)>=10){
        std::cout<<"[PASS] Upper Bound test "<<std::endl;
    }else{
        std::cout<<"[FAIL] Upper Bound test "<<std::endl;
    }
}

void test3(){
    CountMinSketch cms(20,5);
    for(int i =0;i<10;i++){
        cms.incrementHash(2);
    }
    cms.clear();
    for(int i =0;i<5;i++){
        cms.incrementHash(2);
    }
    if(cms.estimateUpperBoundOfHash(2)==5){
        std::cout<<"[PASS] Counters clear test "<<std::endl;
    }else{
        std::cout<<"[FAIL] Counters clear test "<<std::endl;
    }
}

void test4(){
    CountMinSketch cms1(20,5);
    CountMinSketch cms2(20,5);
    for(int i =0;i<10;i++){
        cms1.incrementHash(5);
        cms2.incrementHash(5);
    }
    try{
        cms1.merge(cms2);
        if(cms1.estimateUpperBoundOfHash(5)>=20){
            std::cout<<"[PASS] Merge test "<<std::endl;
        }else{
            std::cout<<"[FAIL] Merge test "<<std::endl;
        }

    }catch(...){
        std::cout<<"[FAIL] Merge test "<<std::endl;
    }
}



int main(){
    test0();
    test1();
    test2();
    test3();
    test4();
}