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





/// \brief Is the searchpos a road in this hurkamatrix? (where 1=road, 0= something else)
/// \return true if road is in matrix, false otherwise
 // (--) test
bool HurkaMatrix::roadAtRelPos(HPos *searchpos, int debugLevel)
{

    if(searchpos->rel_iso_y < 0) {
        if(debugLevel >=1) {
            std::cout << "ERROR " << cn << " providing rel_iso_y < 0\n";
        }
        return false;
    }


    if(searchpos->rel_iso_x < 0) {
        if(debugLevel >=1) { std::cout << "ERROR " << cn << " providing rel_iso_x < 0\n"; }
        return false;
    }



    if(searchpos->rel_iso_y > (rows-1)) {
        if(debugLevel >=1) { std::cout << "clicked position cannot be in this roadnet,  providing rel_iso_y > (matrix.rows-1)       (" << searchpos->rel_iso_y << " vs " << (rows-1) << "!\n"; }
        return false;
    }


    if(searchpos->rel_iso_x > (cols-1)) {
        if(debugLevel >=1) { std::cout << "clicked position cannot be in this roadnet,  providing rel_iso_x > (matrix.cols-1)       (" << searchpos->rel_iso_x << " vs " << (cols-1) << "!\n"; }
        return false;
    }

    /*
    std::cout << "\n\n";
    std::cout << "    Clicked on (rel_iso): " << searchpos->relToString() << "\n";
    std::cout << "    matrix size: " << rows << ", " << cols << "\n";
    std::cout << "    Reading matrix[" << searchpos->rel_iso_y << "][" << searchpos->rel_iso_x << "]\n";
    std::cout << "\n\n";
*/


    if(matrix[searchpos->rel_iso_y][searchpos->rel_iso_x] == 1)
    {
        return true;
    }

    return false;
}






// (++)
void HurkaMatrix::dump(std::string indent)
{
   dumpMatrix(matrix, rows, cols, indent);
}

void HurkaMatrix::dump()
{
   dumpMatrix(matrix, rows, cols, "");
}




