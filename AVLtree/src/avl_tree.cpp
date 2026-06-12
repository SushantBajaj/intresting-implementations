#include "avl_node.h"
#include "avl_tree.h"

#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <ostream>
#include <stdexcept>
#include <queue>
#include <sys/types.h>
#include <cassert>
#include <unistd.h>



AVLtree::AVLtree(){}

void AVLtree::destroy(AVLnode* root){
    if(root==nullptr) return;
    destroy(root->leftChild_);
    destroy(root->rightChild_);
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

void AVLtree::rebalanceFrom(AVLnode* node){
    while(node!=nullptr){
        node->updateHeight();
        AVLnode* nodeParent = node->parent_;
        bool nodePositionIsLeft = (nodeParent==nullptr || nodeParent->leftChild_==node);
        if(node->leftHeight_+1<node->rightHeight_){ //right heavy
            
            AVLnode* tempRightChild = node->rightChild_;
            
            if(tempRightChild->rightHeight_ >= tempRightChild->leftHeight_){ //rr
                
                if(nodeParent==nullptr) this->root_ = rotateLeft(node);
                else if(nodePositionIsLeft) nodeParent->leftChild_ = rotateLeft(node);
                else nodeParent->rightChild_ = rotateLeft(node);
                
            }else{ //rl
                
                node->rightChild_ = rotateRight(node->rightChild_);
                if(nodeParent==nullptr) this->root_ = rotateLeft(node);
                else if(nodePositionIsLeft) nodeParent->leftChild_ = rotateLeft(node);
                else nodeParent->rightChild_ = rotateLeft(node);
            }
        }else if(node->rightHeight_+1<node->leftHeight_){ //left heavy
            
            AVLnode* tempLeftChild = node->leftChild_;
            if(tempLeftChild->leftHeight_>=tempLeftChild->rightHeight_){ //ll
                
                if(nodeParent==nullptr) this->root_ = rotateRight(node);
                else if(nodePositionIsLeft) nodeParent->leftChild_ = rotateRight(node);
                else nodeParent->rightChild_ = rotateRight(node);
                
            }else{ //lr
                
                node->leftChild_ = rotateLeft(node->leftChild_);
                if(nodeParent==nullptr) this->root_ = rotateRight(node);
                else if(nodePositionIsLeft) nodeParent->leftChild_ = rotateRight(node);
                else nodeParent->rightChild_ = rotateRight(node);
                
            }
        }
        node = nodeParent;
    }
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
    rebalanceFrom(curr);
    return true;
}

NodeHeightObj AVLtree::getPredecessor(AVLnode* node){
    AVLnode* curr = node->leftChild_;
    uint64_t height = 1;
    while(curr->rightChild_!=nullptr){
        curr = curr->rightChild_;
        height++;
    }
    return {curr,height};
}
NodeHeightObj AVLtree::getSuccessor(AVLnode* node){
    AVLnode* curr = node->rightChild_;
    uint64_t height = 1;
    while(curr->leftChild_!=nullptr){
        curr = curr->leftChild_;
        height++;
    }
    return {curr,height};
}


int64_t AVLtree::promoteNode(AVLnode* node){
    int64_t val = node->value_;

    AVLnode* nodeParent = node->parent_;
    AVLnode* tempLeftChild = node->leftChild_;
    AVLnode* tempRightChild = node->rightChild_;
    bool nodePositionIsLeft = (nodeParent->leftChild_==node);
    delete node;
    if(nodePositionIsLeft){
        nodeParent->leftChild_ = (tempLeftChild)?(tempLeftChild):(tempRightChild);
        if(tempLeftChild||tempRightChild) nodeParent->leftChild_->parent_ = nodeParent;
    }else{
        nodeParent->rightChild_ = (tempLeftChild)?(tempLeftChild):(tempRightChild);
        if(tempLeftChild||tempRightChild) nodeParent->rightChild_->parent_ = nodeParent;
    }
    return val;



    
}


bool AVLtree::deleteNode(int64_t x){

    AVLnode* curr = root_;
    while (curr->value_!=x) {
        if(x<curr->value_){
            if(curr->leftChild_!=nullptr) curr = curr->leftChild_;
            else break;
        }else{
            if(curr->rightChild_!=nullptr) curr = curr->rightChild_;
            else break;
        }
    }
    if(curr->value_!=x) return false;
    
    AVLnode* lastAffectedNode {};
    AVLnode* currParent = curr->parent_;
    bool currPositionIsLeft = (currParent==nullptr || currParent->leftChild_==curr);

    if(curr->leftChild_==nullptr && curr->rightChild_==nullptr){
        delete curr;
        lastAffectedNode = currParent;
        if(currParent==nullptr){
            root_ = nullptr;
        }else if(currPositionIsLeft){
            currParent->leftChild_ = nullptr;
        }else{
            currParent->rightChild_ = nullptr;
        }

    }else if(curr->leftChild_==nullptr || curr->rightChild_ ==nullptr){
        AVLnode* tempLeftChild = curr->leftChild_;
        AVLnode* tempRightChild = curr->rightChild_;
        delete curr;
        lastAffectedNode = currParent;
        if(currParent==nullptr){
            root_ = (tempLeftChild)?(tempLeftChild):(tempRightChild);
            root_->parent_ = nullptr;
        }else if(currPositionIsLeft){
            currParent->leftChild_ = (tempLeftChild)?(tempLeftChild):(tempRightChild);
            currParent->leftChild_->parent_ = currParent;
        }else{
            currParent->rightChild_ = (tempLeftChild)?(tempLeftChild):(tempRightChild);
            currParent->rightChild_->parent_ = currParent;
        }
        
    }else{
        NodeHeightObj succesor = getSuccessor(curr);
        NodeHeightObj predecessor = getPredecessor(curr);
        AVLnode* NodeToPromote = (succesor.height>predecessor.height)?(succesor.node):(predecessor.node);
        lastAffectedNode = NodeToPromote->parent_;
        int64_t val = promoteNode(NodeToPromote);

        curr->value_ = val;

    }


    rebalanceFrom(lastAffectedNode);
    return true;


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

bool AVLtree::verifyBST(AVLnode* root, int64_t mx, int64_t mn){
    if(root==nullptr) return true;
    
    bool valcheck = (root->value_<mx && root->value_>mn);
    bool left = verifyBST(root->leftChild_,root->value_,mn);
    bool right = verifyBST(root->rightChild_,mx,root->value_);

    return (left && right && valcheck);

}

bool AVLtree::verifyBST(){
    return verifyBST(this->root_, std::numeric_limits<int64_t>::max(), std::numeric_limits<int64_t>::min());
}
bool AVLtree::verifyHeightDiff(){
    return verifyHeightDiff(this->root_);
}