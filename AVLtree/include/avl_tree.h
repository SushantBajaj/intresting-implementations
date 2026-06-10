#pragma once

#include <cstdint>

#include "avl_node.h"

class AVLtree{
    public:
    AVLtree();
    ~AVLtree();

    bool insertNode(int64_t x);
    bool deleteNode(int64_t x);
    void printTree();
    bool verifyBST();
    bool verifyHeightDiff();
    
    private:
    
    
    bool verifyBST(AVLnode* root);
    bool verifyHeightDiff(AVLnode* root);
    AVLnode* rotateLeft(AVLnode* node);
    AVLnode* rotateRight(AVLnode* node);
    void destroy(AVLnode* root);
    AVLnode* root_;

};