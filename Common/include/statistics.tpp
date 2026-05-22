#include <cassert>
#include <cmath>

template<typename Container>
double Mean(Container &arr){
    assert(arr.size()>0);
    double sum = 0;
    size_t count = 0;
    for(auto &i : arr){
        sum+=i;
        count++;
    }
    return sum/count;
}

template<typename Container>
double HarmonicMean(Container &arr){
    assert(arr.size()>0);
    double sum = 0;
    size_t count = 0;
    for(auto &i : arr){
        assert(i!=0);
        sum+= (1.0/i);
        count++;
    }
    return count/sum;
}

template<typename Container>
double GeometricMean(Container &arr){
    assert(arr.size()>0);
    double mult = 1;
    size_t count = 0;
    for(auto &i : arr){
        assert(i>0);
        mult*= i;
        count++;
    }
    return std::pow(mult,1.0/count);
}
