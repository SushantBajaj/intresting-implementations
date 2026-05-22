#include "hyper_log_log.h"
#include "statistics.h"
#include <iostream>
#include <cassert>
#include <algorithm>
#include <cmath>
#include <bitset>
#include <stdexcept>

HyperLogLog::HyperLogLog(int register_bits) : register_bits(register_bits){
    bits_in_hash = sizeof(size_t)*8;

    if(register_bits<4){
        throw std::runtime_error(
            "HLL implementation requires 4 or more register bits"
            );
    }
    if(register_bits>bits_in_hash){
        throw std::runtime_error(
            "register_bits exceeds hash size"
        );
    }
    registers = (size_t)1<<register_bits;
    sketch = std::vector<int>(registers);
    calculateAlpha();
}

void HyperLogLog::calculateAlpha(){

    if (registers==16){
        alpha = 0.673;
    }
    else if (registers == 32){
        alpha = 0.697;
    }
    else if (registers == 64){
        alpha = 0.709;
    }
    else{
        alpha = 0.7213 / (1 + (1.079 / registers));
    }
}

int HyperLogLog::leadingZeros(size_t hash_value,int n){
    int count = 0;

    for(int i =(bits_in_hash-n)-1;i>=0;i--){
        if((((size_t)1<<i)&hash_value) == 0){
            count++;
        }else{
            break;
        }
    }
    return count+1;
}

size_t HyperLogLog::getMsb(size_t hash_value, int n){
    return (hash_value>>(bits_in_hash-n));

}


void HyperLogLog::insert(size_t hash_value){

    size_t register_id = (registers==0)?(0):(getMsb(hash_value,register_bits));
    sketch[register_id] = std::max(sketch[register_id], leadingZeros(hash_value,register_bits));
}

long long HyperLogLog::estimateCardinality(){
    std::vector<double>values(sketch.size());
    for(size_t i =0;i<sketch.size();i++){
        values[i] = std::pow(2,sketch[i]);
    }

    return llround(HarmonicMean(values)) * alpha * registers;
}


int HyperLogLog::TestleadingZeros(size_t hash_value, int n){
    return leadingZeros(hash_value,n);
}
size_t HyperLogLog::TestgetMsb(size_t hash_value, int n){
    return getMsb(hash_value,n);
}



