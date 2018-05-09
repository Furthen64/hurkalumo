#ifndef HURKAMATRIX_H
#define HURKAMATRIX_H

#include "Utils.hpp"

// VERSION 2            2018-03-20



// Hey! If you update the int **matrix object, MAKE SURE you also update the correct rows and cols! A SEGMENTATION FAULT warning.

class HurkaMatrix
{

public:

    HurkaMatrix(int _rows, int _cols);
    ~HurkaMatrix();
    bool isAllocated() { return allocated; }
    void dump(std::string indent);
    void dump();
    int **matrix;
    int rows;
    int cols;

private:
    bool allocated;
    std::string cn = "HurkaMatrix.cpp";

};

#endif
