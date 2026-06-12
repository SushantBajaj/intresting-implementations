#pragma once

#include <cstdint>

#include "avl_node.h"

struct NodeHeightObj{
    AVLnode* node;
    uint64_t height;
};

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
    

    bool verifyBST(AVLnode* root, int64_t mx, int64_t mn);
    bool verifyHeightDiff(AVLnode* root);
    AVLnode* rotateLeft(AVLnode* node);
    AVLnode* rotateRight(AVLnode* node);
    void destroy(AVLnode* root);
    void rebalanceFrom(AVLnode* node);
    NodeHeightObj getPredecessor(AVLnode* node);
    NodeHeightObj getSuccessor(AVLnode* node);
    int64_t promoteNode(AVLnode* node);


    AVLnode* root_;

};