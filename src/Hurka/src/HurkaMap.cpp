#include "HurkaMap.hpp"

// (-+)
HurkaMap::HurkaMap(std::string _mapName, int **_matrix, int mtxRows, int mtxCols)
{

    mapName = _mapName;
    matrix = _matrix;
    matrixRows = mtxRows;
    matrixCols = mtxCols;
}


// TEST
// (--)
void HurkaMap::draw(RenderTarget& rt)
{

    // iterate over all items
     for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
     {
       (*itAll)->draw(rt);
     }

}

// (++)
void  HurkaMap::putBlockList(std::list<Block *> _blockList)
{
    blockList = _blockList;

    // Update Rows n Cols

}


// (--)
// felhantering vid tom lista t.ex
Block *HurkaMap::getBlock()
{

    Block *blockRef = nullptr;

    for (std::list<Block *>::iterator itAll = blockList.begin(); itAll != blockList.end(); ++itAll)
    {

        blockRef = (*itAll);
    }

    return blockRef;
}



// (--)
// TEST
    // Parse the matrix
    // Make a matrix  with only 1s and 0s

HurkaMatrix *HurkaMap::getRoadMatrix()
{


    std::cout << "getRoadMatrix() with matrixRows= " << matrixRows << ", matrixCols= " << matrixCols << "\n";


    HurkaMatrix *newMatrix = new HurkaMatrix(matrixRows, matrixCols);


    for(int y = 0; y < matrixRows; y++)
    {

        for(int x = 0; x < matrixCols; x++)
        {


            // Is it a road?
            if(matrix[y][x] == 101 ||
               matrix[y][x] == 102 ||
               matrix[y][x] == 103 ||
               matrix[y][x] == 104 ||
               matrix[y][x] == 105 ||
               matrix[y][x] == 106 ||
               matrix[y][x] == 107 ) {

                   newMatrix->matrix[y][x] = 1;

               }
               else
               {
                   // Another regular block
                   newMatrix->matrix[y][x] = 0;

               }

        }

    }


    std::cout << " newMatrix " << newMatrix->rows << ", " << newMatrix->cols << "\n";


    return newMatrix;


}



HurkaMap::~HurkaMap()
{
    // Delete the matrix
    if (matrixRows>0) delete [] matrix[0];
    delete [] matrix;
}
