#include "avl_node.h"
#include "avl_tree.h"
#include <cstdint>
#include <iostream>
#include <random>
#include <cstdlib>


uint64_t Random64(){
    static std::mt19937_64 rng(std::random_device{}());
    return rng();
}


void test1(){
    AVLtree avt;
    for(int i =0;i<1000000;i++){
        avt.insertNode(Random64());
    }
    if(avt.verifyBST() && avt.verifyHeightDiff()){
        avt.printTree();
    }
    

}




int main(){
    test1();
    return 0;
}