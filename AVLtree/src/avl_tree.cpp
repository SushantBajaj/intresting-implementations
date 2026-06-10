#include "avl_node.h"
#include "avl_tree.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <queue>
#include <sys/types.h>
#include <cassert>



AVLtree::AVLtree(){}

void AVLtree::destroy(AVLnode* root){
    if(root->leftChild_!=nullptr) destroy(root->leftChild_);
    if(root->rightChild_!=nullptr) destroy(root->rightChild_);
    delete root;
}

AVLtree::~AVLtree(){
    destroy(root_);
}



AVLnode* AVLtree::rotateRight(AVLnode* node){
    if(node->leftChild_==nullptr){
        throw std::invalid_argument("The left child of Node must not be nullptr");
    }
    AVLnode* nodeParent = node->parent_;
    
    AVLnode* left = node->leftChild_;

    node->leftChild_ = node->leftChild_->rightChild_;
    if(node->leftChild_!=nullptr) node->leftChild_->parent_  = node;

    left->rightChild_ = node;
    node->parent_ = left;

    left->parent_ = nodeParent;
    
    node->updateHeight();
    left->updateHeight();
    
    return left;
}

AVLnode* AVLtree::rotateLeft(AVLnode* node){
    if(node->rightChild_==nullptr){
        throw std::invalid_argument("The right child of Node must not be nullptr");
    }
    AVLnode* nodeParent = node->parent_;
    
    AVLnode* right = node->rightChild_;

    node->rightChild_ = node->rightChild_->leftChild_;
    if(node->rightChild_!=nullptr)node->rightChild_->parent_ = node;

    right->leftChild_ = node;
    node->parent_ = right;
    
    right->parent_ = nodeParent;

    node->updateHeight();
    right->updateHeight();

    return right;
}

bool AVLtree::insertNode(int64_t x){
    bool inserted {};
    if(root_==nullptr){
        root_ = new AVLnode(x,nullptr);
        inserted = true;
        return inserted;
    }
    AVLnode* curr = root_;
    while(true){
        if(x<=curr->value_){
            if(curr->leftChild_!=nullptr){
                curr = curr->leftChild_;
            }else{
                break;
            }
        }else{
            if(curr->rightChild_!=nullptr){
                curr = curr->rightChild_;
            }else{
                break;
            }
        } 

    }
    if(x<curr->value_){
        curr->leftChild_ = new AVLnode(x,curr);
        inserted = true;
    }else if(x>curr->value_){
        curr->rightChild_ = new AVLnode(x,curr);
        inserted = true;
    }else{
        inserted = false;
    }
    if(inserted){
        while(curr!=nullptr){
            curr->updateHeight();
            AVLnode* currParent = curr->parent_;
            bool currPositionIsLeft = (currParent==nullptr || currParent->leftChild_==curr);
            if(curr->leftHeight_+1<curr->rightHeight_){ //right heavy

                AVLnode* tempRightChild = curr->rightChild_;
 
                if(tempRightChild->rightHeight_ > tempRightChild->leftHeight_){ //rr

                    if(currParent==nullptr) this->root_ = rotateLeft(curr);
                    else if(currPositionIsLeft) currParent->leftChild_ = rotateLeft(curr);
                    else currParent->rightChild_ = rotateLeft(curr);

                }else{ //rl

                    curr->rightChild_ = rotateRight(curr->rightChild_);
                    if(currParent==nullptr) this->root_ = rotateLeft(curr);
                    else if(currPositionIsLeft) currParent->leftChild_ = rotateLeft(curr);
                    else currParent->rightChild_ = rotateLeft(curr);
                }
            }else if(curr->rightHeight_+1<curr->leftHeight_){ //left heavy

                AVLnode* tempLeftChild = curr->leftChild_;
                if(tempLeftChild->leftHeight_>tempLeftChild->rightHeight_){ //ll

                    if(currParent==nullptr) this->root_ = rotateRight(curr);
                    else if(currPositionIsLeft) currParent->leftChild_ = rotateRight(curr);
                    else currParent->rightChild_ = rotateRight(curr);

                }else{ //lr

                    curr->leftChild_ = rotateLeft(curr->leftChild_);
                    if(currParent==nullptr) this->root_ = rotateRight(curr);
                    else if(currPositionIsLeft) currParent->leftChild_ = rotateRight(curr);
                    else currParent->rightChild_ = rotateRight(curr);

                }
            }
            curr = currParent;
        }
    }
    return inserted;
}


bool AVLtree::deleteNode(int64_t x){
    
}


void AVLtree::printTree(){
    std::queue<AVLnode*> nodeQueue;
    nodeQueue.push(root_);
    while(true){
        uint64_t nullCount = 0;
        size_t nodeQueueSize = nodeQueue.size();
        for(size_t i {};i<nodeQueueSize;i++){
            if(nodeQueue.front()==nullptr){
                std::cout<<"_ ";
                nodeQueue.pop();
                nodeQueue.push(nullptr);
                nodeQueue.push(nullptr);
                nullCount+=2;
            }else{
                AVLnode* curr = nodeQueue.front();
                std::cout<<curr->value_<<" ";
                nodeQueue.push(curr->leftChild_);
                nodeQueue.push(curr->rightChild_);
                if(curr->leftChild_==nullptr) nullCount++;
                if(curr->rightChild_==nullptr) nullCount++;
                nodeQueue.pop();
            }
        }
        std::cout<<std::endl;
        if(nodeQueue.size()==nullCount) break;
    }
    
}

bool AVLtree::verifyHeightDiff(AVLnode* root){
    if(root==nullptr) return true;
    bool left = verifyHeightDiff(root->leftChild_);
    bool right = verifyHeightDiff(root->rightChild_);
    
    bool curr {};
    if(root->leftHeight_+1>=root->rightHeight_ && 
        root->rightHeight_+1>=root->leftHeight_){
        
        curr=true;
    }
    return (curr && left && right);
}

bool AVLtree::verifyBST(AVLnode* root){
    if(root==nullptr) return true;
    bool left = verifyBST(root->leftChild_);
    bool right = verifyBST(root->rightChild_);

    bool curr {};

    if((root->leftChild_==nullptr || root->value_ > root->leftChild_->value_) &&
        (root->rightChild_==nullptr || root->value_ < root->rightChild_->value_)){
        
        curr = true;
    }

    return (curr && left && right);

}

bool AVLtree::verifyBST(){
    return verifyBST(this->root_);
}
bool AVLtree::verifyHeightDiff(){
    return verifyHeightDiff(this->root_);
}