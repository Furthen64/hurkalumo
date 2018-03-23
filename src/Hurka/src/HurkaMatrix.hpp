#ifndef HURKAMATRIX_H
#define HURKAMATRIX_H

#include "Utils.hpp"

// VERSION 2            2018-03-20




class HurkaMatrix
{

public:

    HurkaMatrix(int _rows, int _cols);
    ~HurkaMatrix();
    bool isAllocated() { return allocated; }
    void dump();
    int **matrix;
    int rows;
    int cols;

private:
    bool allocated;

};

#endif
