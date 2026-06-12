#include "avl_tree.h"
#include <cstdint>
#include <random>
#include <vector>


uint64_t Random64(){
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}




void test1(){
    AVLtree avt;
    std::vector<uint64_t> arr;
    for(int i =0;i<1000000;i++){
        int64_t num = Random64();
        arr.push_back(num);
        avt.insertNode(num);
    }
    for(auto i : arr){
        if(i%2==0){
            avt.deleteNode(i);
        }
    }

    if(avt.verifyBST() && avt.verifyHeightDiff()){
        avt.printTree();
    }

}




int main(){
    test1();
    return 0;
}