#include "BinarySearchTree.hpp"
BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
    nrElements = 0;
}

void BinarySearchTree::dump()
{
    std::cout << "\n\nDumping BinarySearchTree (" << nrElements << " elements)" << ":\n";
    dumpBST(root, -1);
}



// Does it worK???
//
// Ugly.
// See "Wishlist Beta" in Docs/ folder
// RECURSIVE
// (--) TEST
void BinarySearchTree::dumpBST(BSTNode *currNode, int direction)
{
    BSTNode *nextNode = nullptr;

    if(currNode == nullptr) {
        std::cout << "trying to dump nullptr\n";
        return ;
    }
    std::string pre = "";


    if(direction == -1) {
            pre = "";
    } else if(direction == 0) {
        pre = "-";
    } else if(direction == 1) {
        pre = "+";
    }

    std::cout << "             " << pre << "(" << currNode->val << ")\n";


    if(currNode->left != nullptr) {
        nextNode = currNode->left;
        dumpBST(nextNode, 0);
    }

    if(currNode->right != nullptr) {
        nextNode = currNode->right;
        dumpBST(nextNode, 1);
    }
}

// Does it work??
// Dump shows weird things...
// (--)
int BinarySearchTree::add(int x, int debugLevel)
{

    if(x < 0) {
        std::cout << "ERROR cannot search for values < 0 !\n";
        return -1;
    }

    int val = 0;
    bool found = false;
    BSTNode *currNode = root;
    std::string ind = "";

    if(root == nullptr) {
        root = new BSTNode(x);

        if(debugLevel >= 1) { std::cout << "Added root node in BST\n"; }
        nrElements++;
        return 0;
    }


    // Find whre in the tree we should put this one
    while(!found) {

        if(x == currNode->val) {
            // Do nothing, we already have this value in the Tree
            found = true;

        } else if(x > currNode->val) {

            if(currNode->right != nullptr) {
                ind += "+";
                currNode = currNode->right;
            } else {
                if(debugLevel >= 1) { std::cout << ind << "Added right node in BST (" << x << ")\n"; }

                currNode->right = new BSTNode(x);

                nrElements++;

                found = true;
            }

        } else {

            if(currNode->left != nullptr) {
                ind += "-";
                currNode = currNode->left;
            } else {

                if(debugLevel >= 1) {std::cout << ind << "Added left node in BST (" << x << ")\n";}

                currNode->left = new BSTNode(x);

                nrElements++;

                found = true;
            }
        }
    }
    return val;


}


/// Only positive integers
/// Return found value if found
/// Return -1 if not found
// (-+)
int BinarySearchTree::findVal(int x, int debugLevel)
{
    if(x < 0) {
        std::cout << "ERROR cannot search for values < 0 !\n";
        return -1;
    }


    int val = 0;
    bool found = false;
    BSTNode *currNode;

    if(root == nullptr) {
            if(debugLevel >=1) {std::cout << "Warning! searching an empty binary search tree. Root is nullptr\n";}
            return -1;
    }

    currNode = root;

    while(!found) {

        if(currNode != nullptr) {
            if(debugLevel >= 1)  { std::cout << "currNode.val=" << currNode->val << "\n"; }
        }


        if(x == currNode->val) {
                // Do nothing, we already have this value in the Tree
                val = x;
                found = true;
        } else if(x > currNode->val) {

            if(currNode->right != nullptr) {
                currNode = currNode->right;
            } else {
                if(debugLevel >= 1)  { std::cout << "Gave up on finding " << x << "\n";}
                val = -1;
                found = true;
            }
        } else {
            if(currNode->left != nullptr) {
                currNode = currNode->left;
            } else {
                if(debugLevel >= 1)  { std::cout << "Gave up on finding " << x << "\n";}
                val = -1;
                found = true;
            }
        }



    }

    if(!found) {
        std::cout << "ERROR Could not find the value " << x << " in the BinarySearchTree\n";

        return -1;
    }

    return val;
}
