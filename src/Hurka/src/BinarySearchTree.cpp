#include "BinarySearchTree.hpp"
BinarySearchTree::BinarySearchTree()
{
    root = nullptr;
}

void BinarySearchTree::dumpBST()
{
    std::cout << "\n\nDumping BinarySearchTree:\n";
    dump(root);
}


// TEST
// (--)
void BinarySearchTree::dump(BSTNode *currNode)
{
    if(currNode == nullptr) {
        std::cout << "trying to dump nullptr\n";
        return ;
    }

    std::cout << "             (" << currNode->val << ")\n";


    if(currNode->left != nullptr) {
        currNode = currNode->left;
        dump(currNode);
    }

    if(currNode->right != nullptr) {
        currNode = currNode->right;
        dump(currNode);
    }
}

// Seems to work
// (-+)
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
        return 0;
    }


    // Find whre in the tree we should put this one
    while(!found) {

        if(x == currNode->val) {
                // Do nothing, we already have this value in the Tree
                found = true;
        } else if(x > currNode->val) {

            if(currNode->right != nullptr) {
                currNode = currNode->right;
                ind += "+";
            } else {
                currNode->right = new BSTNode(x);
                if(debugLevel >= 1) { std::cout << ind << "Added right node in BST (" << x << ")\n"; }

                found = true;
            }
        } else {
            if(currNode->left != nullptr) {
                ind += "-";
                currNode = currNode->left;
            } else {
                if(debugLevel >= 1) {std::cout << ind << "Added left node in BST (" << x << ")\n";}

                currNode->left = new BSTNode(x);
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
            std::cout << "Warning! searching an empty binary search tree. Root is nullptr\n";
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
