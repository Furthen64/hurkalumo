#include "HurkaMatrix.hpp"

HurkaMatrix::HurkaMatrix(int _rows, int _cols)
{
    std::cout << "allocating matrix with rows=" << _rows << ", cols=" << _cols << "\n";
   matrix = allocateMatrix(_rows, _cols);
   rows = _rows;
   cols = _cols;
   allocated = true;
}

HurkaMatrix::~HurkaMatrix()
{

}

void HurkaMatrix::dump()
{
   dumpMatrix(matrix, rows, cols);
}




