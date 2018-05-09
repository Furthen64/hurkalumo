#include "HurkaMatrix.hpp"

HurkaMatrix::HurkaMatrix(int _rows, int _cols)
{
    if(_rows < 0 || _cols < 0) {
        std::cout << "ERROR " << cn << " Cannot create a matrix with rows=" << _rows << ", cols=" << _cols << "!\n";
        return ;
    }

    matrix = allocateMatrix(_rows, _cols);
    rows = _rows;
    cols = _cols;
    allocated = true;
}

HurkaMatrix::~HurkaMatrix()
{

}

void HurkaMatrix::dump(std::string indent)
{
   dumpMatrix(matrix, rows, cols, indent);
}

void HurkaMatrix::dump()
{
   dumpMatrix(matrix, rows, cols, "");
}




