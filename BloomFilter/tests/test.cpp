#include <bloom_filter.h>

#include <iostream>

void test0(){
    try{
        BloomFilter bf(100,2);
        std::cout<<"[PASS] Constructor test "<<std::endl;

    }catch(...){
        std::cout<<"[FAIL] Constructor test "<<std::endl;
    }
}
void test1(){
    BloomFilter bf(100,1);
    bf.insertHash(2);
    if(bf.containsHash(2)){
        std::cout<<"[PASS] True positive test "<<std::endl;

    }else{
        std::cout<<"[FAIL] True positive test "<<std::endl;
    }
}
void test2(){
    BloomFilter bf(100,1);
    bf.insertHash(2);
    if(!bf.containsHash(3)){
        std::cout<<"[PASS] Non-member lookup test "<<std::endl;
    }else{
        std::cout<<"[FAIL] Non-member lookup test "<<std::endl;
    }
}

void test3(){
    BloomFilter bf(10000,2);
    for(int i=0;i<1000;i++){
        bf.insertHash(i);
    }

    for(int i=0;i<1000;i++){
        if(!(bf.containsHash(i))){
            std::cout<<"[FAIL] No false negative test "<<std::endl;
            return;
        }
    }
    std::cout<<"[PASS] No false negative test "<<std::endl;
        
}


int main(){
    test0();
    test1();
    test2();
    test3();
}