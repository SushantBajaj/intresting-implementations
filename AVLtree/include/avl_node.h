#pragma once
#include <algorithm>

#include <cstdint>

class AVLnode{
    public:

    AVLnode(int64_t val, AVLnode* parent):
        value_(val),
        parent_(parent) {}
    int64_t value_;
    AVLnode* leftChild_ = {};
    AVLnode* rightChild_ = {};
    AVLnode* parent_ = {};
    uint64_t leftHeight_ = {};
    uint64_t rightHeight_ ={};
    void updateHeight(){
        AVLnode* tempLeftChild = this->leftChild_;
        AVLnode* tempRightChild = this->rightChild_;
        if(tempLeftChild!=nullptr){
            this->leftHeight_ = std::max(tempLeftChild->leftHeight_,tempLeftChild->rightHeight_)+1;
        }else{
            this->leftHeight_ = 0;
        }
        if(tempRightChild!=nullptr){
            this->rightHeight_ = std::max(tempRightChild->leftHeight_, tempRightChild->rightHeight_)+1;
        }else{
            this->rightHeight_ = 0;
        }
    }


};