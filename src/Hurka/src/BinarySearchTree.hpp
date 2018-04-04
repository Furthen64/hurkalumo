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
    void dump(BSTNode *currNode);
    void dumpBST();
    add(int x, int debugLevel);
    findVal(int x, int debugLevel);

private:
    BSTNode *root;
};


#endif
