#ifndef BINARYSEARCHTREE_H
#define BINARYSEARCHTREE_H

#include <string>
#include <iostream>



class BSTNode
{
public:
    BSTNode(int _val)
    {
        val = _val;
        left = nullptr;
        right = nullptr;
    }
    int val; // Unique identifier

    BSTNode *left;
    BSTNode *right;

private:

};




class BinarySearchTree
{
public:
    BinarySearchTree();
    void dump();
    void dumpBST(BSTNode *currNode, int direction);
    int add(int x, int debugLevel);
    int findVal(int x, int debugLevel);

private:
    BSTNode *root;
    int nrElements;
};


#endif
